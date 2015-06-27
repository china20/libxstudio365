#include <queue>
#include <errno.h>
#include "purelib/utils/container_helper.h"

#include "platform/CCFileUtils.h"
// #include "GameConfig.h"
#include "NXHttpClient.h"

using namespace purelib;
using namespace cocos2d;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

// Callback function used by libcurl for collect response data
static size_t write_data_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    NXHttpResp *recvBuffer = (NXHttpResp*)userp;
    size_t sizes = size * nmemb;

    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr + sizes);

    return sizes;
}

static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    NXHttpReq* request = (NXHttpReq*)clientp;
    if(dltotal + ultotal > 1) {
        request->notifyProgress((dlnow + ulnow ) / (dltotal + ultotal));
    }
    return 0;
}

// Callback function used by libcurl for collect header data
//static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
//    size_t sizes = size * nmemb;
//    
//    // add data to the end of recvBuffer
//    // write data maybe called more than once in a single request
//    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
//    
//    return sizes;
//}

////Process Get Request
//static int processGetTask(HttpRequest *request, write_callback callback, void *stream, int32_t *responseCode, write_callback headerCallback, void *headerStream)
//{
//    CURLRaii curl;
//    bool ok = curl.init(request, callback, stream, headerCallback, headerStream)
//            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
//            && curl.perform(responseCode);
//    return ok ? 0 : 1;
//}

NXHttpClient::NXHttpClient(void)
    : timeoutForConnect_(5)
    , timeoutForRead_(30)
    , thread_started_(false)
{
    curl_global_init(CURL_GLOBAL_ALL);

    this->asyncRequestCount = 0;
    
    insertHostUrl("http://42.121.35.53/lovelybeans.api");

    this->curl_.configureCURL(getUrl().c_str(), timeoutForRead_, timeoutForConnect_);
}

bool NXHttpClient::init(void)
{
    scheduleCollectResponse();
    return true;
}

void NXHttpClient::scheduleCollectResponse(void)
{
    Director::getInstance()->getScheduler()->schedule(
        schedule_selector(NXHttpClient::dispatchResponseCallbacks), this, 0, false);
    Director::getInstance()->getScheduler()->pauseTarget(this);
}

void NXHttpClient::unscheduleCollectResponse(void)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(NXHttpClient::dispatchResponseCallbacks), this);
}

void NXHttpClient::insertHostUrl(const char* url)
{
    if( std::find(urls_.begin(), urls_.end(), url) == urls_.end() )
        this->urls_.push_back(url);

    this->urlIdx_ = this->urls_.size() - 1;
}

const std::string& NXHttpClient::getUrl(void) const
{
    return this->urls_[urlIdx_];
}

void NXHttpClient::pollUrl(void)
{
    ++urlIdx_;
    if (urlIdx_ >= urls_.size())
        urlIdx_ = 0;

    bUrlChanged_ = true;
}

NXHttpClient::~NXHttpClient()
{
    need_quit_ = true;
    // Do not need call it
    // CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NXHttpClient::dispatchResponseCallbacks), this);

    if (this->thread_started_) {
        this->requestqMtx_.lock();
        container_helper::clear_c(this->requestq_, std::mem_fun(&CCObject::release));
        this->requestqMtx_.unlock();
        ///this->thread_->detach();
        sleepCondition.notify_one();
        //this->thread_->join();


        //this->thread_->detach();
        /*delete this->thread_;*/
        while(this->thread_started_){
            std::unique_lock<std::mutex> lk(thread_running_mutex);
            thread_running_condition.wait(lk);
        }
    }
}

//Lazy create semaphore & mutex & thread
bool NXHttpClient::lazyInitThreadSemphore()
{
    if (thread_started_) {
        return true;
    } else {

        this->thread_started_ = true;
        need_quit_ = false;

        //this->thread_ = new std::thread(std::bind(&NXHttpClient::networkThread, this));
        auto t = std::thread(std::bind(&NXHttpClient::networkThread, this));
        t.detach();
    }

    return true;
}

void NXHttpClient::send(const std::string& message, const HttpResponseCallback& callback, const HttpProgressCallback& progressCallback)
{
    // this->curl_.setOption(CURLOPT_PORT, getNextPort());
    if (this->bUrlChanged_)
    {
        this->bUrlChanged_ = false;
        this->curl_.configureCURL(getUrl().c_str(), this->timeoutForRead_, this->timeoutForConnect_);
    }

    this->send(message.c_str(), message.size(), callback, progressCallback);
}

//Add a get task to queue
void NXHttpClient::send(const char* buffer, size_t n, const HttpResponseCallback& callback, const HttpProgressCallback& progressCallback)
{    
    if (false == lazyInitThreadSemphore()) 
    {
        return;
    }

    ++asyncRequestCount;

    NXHttpReq* request = new NXHttpReq();
    request->setRequestData(buffer, n);
    request->setCallback(callback, progressCallback);
    // request->retain();

    this->requestqMtx_.lock();
    this->requestq_.push_back(request);
    this->requestqMtx_.unlock();

    // Notify thread start to work
    this->sleepCondition.notify_one();
}

// Poll and notify main thread if responses exists in queue
void NXHttpClient::dispatchResponseCallbacks(float delta)
{
    //// log("CCHttpClient::dispatchResponseCallbacks is running");

    NXHttpResp* response = NULL;

    this->responseqMtx_.lock();

    if (!this->responseq_.empty())
    {
        response = *(this->responseq_.begin());
        this->responseq_.pop_front();
    }

    this->responseqMtx_.unlock();

    if (response)
    {
        --asyncRequestCount;

        if (!response->isSucceed())
            this->pollUrl();

        dynamic_cast<NXHttpResp*>(response)->invoke();

        response->release();
    }

    if (0 == asyncRequestCount) 
    {
        Director::getInstance()->getScheduler()->pauseTarget(this);
    }

}

// Worker thread
void NXHttpClient::networkThread(void)
{    
    NXHttpReq *request = NULL;

    while (true) 
    {
        if (need_quit_)
        {
            break;
        }

        // step 1: send http request if the requestQueue isn't empty
        request = NULL;

        this->requestqMtx_.lock();

        //Get request task from queue

        if (!requestq_.empty())
        {
            request = dynamic_cast<NXHttpReq*>(*this->requestq_.begin());
            this->requestq_.pop_front();
        }
        this->requestqMtx_.unlock();

        if (NULL == request)
        {
            // Wait for http request tasks from main thread
            std::unique_lock<std::mutex> lk(this->sleepMutex); 
            this->sleepCondition.wait(lk);
            continue;
        }

        // step 2: libcurl sync access

        // Create a HttpResponse object, the default setting is http access failed
        NXHttpResp *response = new NXHttpResp(request);
        //    
        //    // request's refcount = 2 here, it's retained by HttpRespose constructor
        // request->release();
        // ok, refcount = 1 now, only HttpResponse hold it.

        int32_t responseCode = -1;
        int retValue = 0;

        retValue = this->processPostTask(request, write_data_callback, response, &responseCode);

        response->setResponseCode(responseCode);
        response->setSucceed(retValue == 0);

        request->release();

        // add response packet into queue
        this->responseqMtx_.lock();
        this->responseq_.push_back(response);
        this->responseqMtx_.unlock();

        // resume dispatcher selector
        Director::getInstance()->getScheduler()->resumeTarget(this);
    }

    asyncRequestCount -= this->requestq_.size();

    this->thread_started_ = false;
    thread_running_condition.notify_one();
    CCLOG("NXHttpClient: thread exiting...");
}

//Process POST Request
int NXHttpClient::processPostTask(NXHttpReq* request, write_callback callback, void* userp, int32_t *responseCode)
{
    bool ok = this->curl_.setOption(CURLOPT_POST, 1)
        && this->curl_.setOption(CURLOPT_WRITEFUNCTION, callback)
        && this->curl_.setOption(CURLOPT_WRITEDATA, userp)
        /*&& this->curl_.setOption(CURLOPT_NOPROGRESS, false)
        && this->curl_.setOption(CURLOPT_PROGRESSFUNCTION, progress_callback)
        && this->curl_.setOption(CURLOPT_PROGRESSDATA, request) progress callback function config*/
        && this->curl_.setOption(CURLOPT_POSTFIELDS, request->data())
        && this->curl_.setOption(CURLOPT_POSTFIELDSIZE, request->size())
        && this->curl_.perform(responseCode);
    return ok ? 0 : 1;
}

void NXHttpClient::enableCookies(const char* cookieFile) {
    /*if (cookieFile) {
    s_cookieFilename = std::string(cookieFile);
    }
    else {
    s_cookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }*/
}

