/****************************************************************************
Copyright (c) 2014 purelib, All Rights Reserved.
****************************************************************************/

#include <queue>
#include <errno.h>
#include "utils/container_helper.h"

#include "platform/CCFileUtils.h"
// #include "GameConfig.h"
#include "NXNetwork.h"
#include "utils/crypto_wrapper.h"

using namespace thelib;
using namespace cocos2d;

static const size_t MAX_NET_DATA_LEN = 8192;

/********************************************************
**
** tcp client impl
**
*********************************************************/
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
namespace purelib { 
class tcp_client
{
public:
    tcp_client(const std::string& address, u_short port)
        : 
        io_service_(),
        service_running_(false),
        socket_(io_service_),
        timeout_check_timer_(io_service_),
        address_(address),
        port_(port),
        max_data_len_(8192),
        connect_timeout_(5),
        request_timeout_(10),

        error_code_(0),
        expecting_size_(0),
        response_buffer_(nullptr),
        total_bytes_read_(0),
        connected_(false)
    {
    }

    void set_response_buffer(std::string* buffer)
    {
        reset();
        response_buffer_ = buffer;
    }

    void start_request(const void* data, size_t len)
    {
        tcp::endpoint ep(boost::asio::ip::address_v4::from_string(address_), port_);
        socket_.open(boost::asio::ip::tcp::v4());

        do_timeout_check(connect_timeout_);
        socket_.async_connect(ep, [this,data,len](const boost::system::error_code& ec){

            cancel_timer();
            if(!ec)
            {
                // The connection was successful. Send the request.
                connected_ = true;
                do_timeout_check(request_timeout_);
                boost::asio::async_write(socket_, 
                    boost::asio::buffer(data, len), 
                    [this](const boost::system::error_code& ec, size_t) {
                        handle_write_request(ec);
                });
            }
            else {
                handle_error(ec);
            }
        });

        if(!service_running_)
        {
            service_running_ = true;
            boost::system::error_code ignored_ec;
            io_service_.reset();
            io_service_.run(ignored_ec);
            this->socket_.close(ignored_ec);
            service_running_ = false;
        }
    }

    int get_error_code(void) const
    {
        return this->error_code_;
    }

private:

    void reset(void)
    {
        error_code_ = 0;
        expecting_size_ = 0;
        response_buffer_ = nullptr;
        total_bytes_read_ = 0; 
        connected_ = false;
    }

    void do_timeout_check(size_t sec)
    {
        timeout_check_timer_.expires_from_now(boost::posix_time::seconds(sec));
        timeout_check_timer_.async_wait([this](const boost::system::error_code& ec){
            if(!ec) {
                this->abort();
            }
        });
    }

    void handle_write_request(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            // Read the response data length
            boost::asio::async_read(socket_, 
                boost::asio::buffer((void*)&expecting_size_, sizeof(expecting_size_)),
                boost::asio::transfer_exactly( sizeof(expecting_size_) ), [this](const boost::system::error_code& ec, size_t){
                    handle_read_length(ec);
            });
        }
        else
        {
            handle_error(ec);
        }
    }

    void handle_read_length(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            expecting_size_ = ntohl(expecting_size_);
            if(expecting_size_ > this->max_data_len_)
            { // too long response received
                cancel_timer();
                return;
            }

            this->total_bytes_read_ = 0;

            // Start reading remaining data until EOF.
            size_t size = (std::min)(sizeof(buffer_), this->expecting_size_);
            boost::asio::async_read(socket_, 
                boost::asio::buffer(buffer_, size ),
                boost::asio::transfer_exactly( size ), [this](const boost::system::error_code& ec, size_t bytes_transferred){
                    handle_read_content(ec, bytes_transferred);
            });
        }
        else
        {
            handle_error(ec);
        }
    }

    void handle_read_content(const boost::system::error_code& ec, size_t bytes_transferred)
    {
        if (!ec)
        {
            // write buffer to response data
            response_buffer_->insert(response_buffer_->end(), buffer_, buffer_ + bytes_transferred);

            this->total_bytes_read_ += bytes_transferred;

            if(this->total_bytes_read_ < this->expecting_size_)
            {
                // Continue reading remaining data until EOF.
                size_t size = (std::min)(sizeof(buffer_), this->expecting_size_ - this->total_bytes_read_);
                boost::asio::async_read(socket_, 
                    boost::asio::buffer(buffer_, size ),
                    boost::asio::transfer_exactly( size ), [this](const boost::system::error_code& ec, size_t bytes_transferred){
                        handle_read_content(ec, bytes_transferred);
                });
            }
            else { // complete
                this->error_code_ = ec.value();
                cancel_timer();
            }
        }
        else
        {
            handle_error(ec);
        }
    }

    void handle_error(const boost::system::error_code& ec)
    {
        this->cancel_timer();
        
        if(ec != boost::asio::error::operation_aborted)
            this->abort();

        error_code_ = ec.value();
    }

public:
    void abort(void)
    {
        boost::system::error_code ignored_ec;
        socket_.close(ignored_ec);
        //io_service_.stop();
        if(!connected_)
        {
            CCLOG("%s", "connect server timeout.");
        }
        else {
            CCLOG("%s", "read response data timeout.");
        }
    }

    void cancel_timer(void)
    {
        boost::system::error_code ignored_ec;
        this->timeout_check_timer_.cancel(ignored_ec);
    }

    boost::asio::io_service     io_service_;

    bool                        service_running_;

    std::string                 address_;
    u_short                     port_;
    tcp::socket                 socket_;
    bool                        connected_;
    int                         error_code_; // error code
    const size_t                max_data_len_; // max network data length

    boost::asio::deadline_timer timeout_check_timer_;
    size_t                      connect_timeout_;
    size_t                      request_timeout_;

    char                        buffer_[512]; // fixed read slice of response buffer
    size_t                      expecting_size_; // expecting response data size

    std::string*                response_buffer_; // external pointer: the real response buffer
    size_t                      total_bytes_read_; // totoal bytes read from server in this request
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

NetClient::NetClient(void)
    : timeoutForConnect(5)
    , timeoutForRead(60)
    , threadStarted(false)
{
    // curl_global_init(CURL_GLOBAL_ALL);

    this->asyncRequestCount = 0;

    addr = "192.168.1.19";

    // ports_.push_back(8080);
    ports.push_back(8080);
    // ports_.push_back(9090);
    portIndex = ports.size() - 1;

    this->client = new tcp_client(addr, 8080);
}

void NetClient::scheduleCollectResponse(void)
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
        schedule_selector(NetClient::dispatchResponseCallbacks), this, 0, false);
    CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
}

void NetClient::unscheduleCollectResponse(void)
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetClient::dispatchResponseCallbacks), this);
}

NetClient::~NetClient()
{
    needQuit = true;
    // Do not need call it
    // CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NetClient::dispatchResponseCallbacks), this);
    this->client->abort();

    if (this->threadStarted) {
        this->requestqMtx.lock();
        container_helper::clear_c(this->requestq, std::mem_fun(&CCObject::release));
        this->requestqMtx.unlock();
        ///this->thread_->detach();
        // sock.close();
        

        sleepCondition.notify_one();
        //this->thread_->join();

        //this->thread_->detach();
        /*delete this->thread_;*/
        while(this->threadStarted){
            std::unique_lock<std::mutex> lk(threadRunningMtx);
            threadRunningCond.wait(lk);
        } 
    }

    delete this->client;
}

//Lazy create semaphore & mutex & thread
bool NetClient::lazyInitThreadSemphore()
{
    if (threadStarted) {
        return true;
    } else {

        this->threadStarted = true;
        needQuit = false;

        //this->thread_ = new std::thread(std::bind(&NetClient::networkThread, this));
        auto t = std::thread(std::bind(&NetClient::networkThread, this));
        t.detach();
    }

    return true;
}

void NetClient::postMessage(const std::string& message, const NetResponseCallback& callback)
{
    // this->curl_.setOption(CURLOPT_PORT, nextPort());
    
    this->postMessage(message.c_str(), message.size(), callback);
}

//Add a get task to queue
void NetClient::postMessage(const char* buffer, size_t n, const NetResponseCallback& callback)
{    
    if (false == lazyInitThreadSemphore()) 
    {
        return;
    }

    ++asyncRequestCount;

    NetRequest* request = new NetRequest();
    request->setRequestData(buffer, n);
    request->setCallback(callback);
    // request->retain();

    this->requestqMtx.lock();
    this->requestq.push_back(request);
    this->requestqMtx.unlock();

    // Notify thread start to work
    this->sleepCondition.notify_one();
}

// Poll and notify main thread if responses exists in queue
void NetClient::dispatchResponseCallbacks(float delta)
{
    //// log("CCHttpClient::dispatchResponseCallbacks is running");

    NetResponse* response = NULL;

    this->responseqMtx.lock();

    if (!this->responseq.empty())
    {
        response = *(this->responseq.begin());
        this->responseq.pop_front();
    }

    this->responseqMtx.unlock();

    if (response)
    {
        --asyncRequestCount;

        dynamic_cast<NetResponse*>(response)->invoke();

        response->release();
    }

    if (0 == asyncRequestCount) 
    {
        CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
    }

}

u_short NetClient::nextPort(void)
{
    ++portIndex;
    if (portIndex == ports.size())
        portIndex = 0;
    return this->ports[portIndex];
}

// Worker thread
void NetClient::networkThread(void)
{    
    NetRequest *request = NULL;

    while (true) 
    {
        if (needQuit)
        {
            break;
        }

        // step 1: send http request if the requestQueue isn't empty
        request = NULL;

        this->requestqMtx.lock();

        //Get request task from queue

        if (!requestq.empty())
        {
            request = dynamic_cast<NetRequest*>(*this->requestq.begin());
            this->requestq.pop_front();
        }
        this->requestqMtx.unlock();

        if (NULL == request)
        {
            // Wait for http request tasks from main thread
            std::unique_lock<std::mutex> lk(this->sleepMutex); 
            this->sleepCondition.wait(lk);
            continue;
        }

        // Create a HttpResponse object, the default setting is http access failed
        NetResponse *response = new NetResponse(request);
        //    
        //    // request's refcount = 2 here, it's retained by HttpRespose constructor
        // request->release();
        // ok, refcount = 1 now, only HttpResponse hold it.

        int32_t responseCode = -1;

        this->client->set_response_buffer(response);
        this->client->start_request(request->getRequestData(), request->getRequestDataSize());
        // retValue = this->processPostTask(request, write_data_callback, response, &responseCode);

        response->setResponseCode(this->client->get_error_code());
        response->setSucceed(this->client->get_error_code() == 0);

        request->release();

        // add response packet into queue
        this->responseqMtx.lock();
        this->responseq.push_back(response);
        this->responseqMtx.unlock();

        // resume dispatcher selector
        CCDirector::sharedDirector()->getScheduler()->resumeTarget(this);
        
    }

    asyncRequestCount -= this->requestq.size();

    this->threadStarted = false;
    threadRunningCond.notify_one();
    CCLOG("NetClient: thread exiting...");
}

};

//void NetClient::reply(NetRequest* request, const boost::system::error_code& ec)
//{
//    auto response = new NetResponse(request);
//    request->release();
//    reply(response, ec);
//}
//
//void NetClient::reply(NetResponse* response, const boost::system::error_code& ec)
//{
//    response->setSucceed(!ec);
//    response->setResponseCode(ec.value());
//
//    // add response packet into queue
//    this->responseqMtx.lock();
//    this->responseq.push_back(response);
//    this->responseqMtx.unlock();
//
//    // resume dispatcher selector
//    CCDirector::sharedDirector()->getScheduler()->resumeTarget(this);
//}



