/****************************************************************************
Copyright (c) 2014 purelib. All rights reserved.
Responsible for network transfer and data encrypt

client: 
encrypt data as AES
RSA_sign(AES_key, private_key) and append to data
server RSA_verify(AES_key, server_key) 
use AES_Key to decrypt client data.

****************************************************************************/

#ifndef _NXNETWORK_H_
#define _NXNETWORK_H_
#if defined(_WIN32) && _MSC_VER < 1700
#include "utils/thread.h"
#else
#include <mutex>

#include <thread>
#include <condition_variable>
#include <list>
#include <vector>
#endif
#include <cocos2d.h>

#include "utils/singleton.h"

using namespace purelib::gc;

namespace purelib {

enum NetErrorCode 
{
    NoError,
    ConnectFailed,
    SendFailed,
    RecvFailed
};

class NetResponse;
typedef std::function<void(NetResponse* response)> NetResponseCallback;

class NetObject : public cocos2d::CCObject, public std::string
{
};

class NetRequest : public NetObject
{
public:
    friend class NetResponse;
    /** Constructor
    Because HttpRequest object will be used between UI thead and network thread,
    requestObj->autorelease() is forbidden to avoid crashes in AutoreleasePool
    new/retain/release still works, which means you need to release it manually
    Please refer to HttpRequestTest.cpp to find its usage
    */
    NetRequest()
    {
    };

protected:
    /** Destructor */
    virtual ~NetRequest()
    {
    };

public:
    // setter/getters for properties

    /** Required field for HttpRequest object before being sent.
    kHttpGet & kHttpPost is currently supported
    */
    inline void setRequestType(int type)
    {
        type_ = type;
    };
    /** Get back the kHttpGet/Post/... enum value */
    inline int getRequestType()
    {
        return type_;
    };


    /** Option field. You can set your post data here
    */
    inline void setRequestData(const char* buffer, unsigned int len)
    {
        this->resize(sizeof(len));
        * ( (uint32_t*) &this->front() ) = htonl(len);
        this->append(buffer, buffer + len);
        // this->copy(buffer, len);
    };
    /** Get the request data pointer back */
    inline const char* getRequestData()
    {
        return this->data();
    }
    /** Get the size of request data back */
    inline int getRequestDataSize()
    {
        return this->size();
    }

    void setCallback(const NetResponseCallback& responseCallback)
    {
        this->response_callback_ = responseCallback;
    }

protected:
    // properties
    int                        type_;    /// kHttpRequestGet, kHttpRequestPost or other enums
    NetResponseCallback        response_callback_;
};

class tcp_client;

class NetResponse : public NetObject
{
public:
    NetResponse(NetRequest* request) : succeed_(false)
    {
        CC_ASSERT(request);
        request->retain();
        this->request_ = request;
    }
    virtual ~NetResponse(void)
    {
        request_->release();
    }

    void setResponseCode(int value)
    {
        this->responseCode_ = value;
    }

    int getResponseCode(void) const
    {
        return this->responseCode_;
    }

    void setSucceed(bool succeed)
    {
        this->succeed_ = succeed;
    }

    bool isSucceed(void) const
    {
        return this->succeed_;
    }

    void invoke(void)
    {
        request_->response_callback_(this);
    }

protected:
    int                         responseCode_;
    bool                        succeed_;
    NetRequest*               request_;
};

/** @brief Singleton that handles asynchrounous http requests
* Once the request completed, a callback will issued in main thread when it provided during make request
* Must call scheduleCollectResponse when start and unscheduleCollectResponse when application ending
*      
*/
class NetClient : public cocos2d::CCObject
{
public:
    NetClient();
    virtual ~NetClient();

    void scheduleCollectResponse(void);
    void unscheduleCollectResponse(void);

    /**
    * Add a get request to task queue
    * @param request a HttpRequest object, which includes url, response callback etc.
    please make sure request->_requestData is clear before calling "send" here.
    */
    void postMessage(const char* buffer, size_t len, const NetResponseCallback& callback);
    void postMessage(const std::string& message, const NetResponseCallback& callback);


    /**
    * Change the connect timeout
    * @param value The desired timeout.
    */
    inline void setTimeoutForConnect(int value) {timeoutForConnect = value;};

    /**
    * Get connect timeout
    * @return int
    */
    inline int getTimeoutForConnect() {return timeoutForConnect;}


    /**
    * Change the download timeout
    * @param value
    */
    inline void setTimeoutForRead(int value) {timeoutForRead = value;};


    /**
    * Get download timeout
    * @return int
    */
    inline int getTimeoutForRead() {return timeoutForRead;};

    void networkThread(void);
private:

    // int  processPostTask(NetRequest* request, write_callback callback, void *stream, int32_t *responseCode);

    /**
    * Init pthread mutex, semaphore, and create new thread for http requests
    * @return bool
    */
    bool lazyInitThreadSemphore();
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks(float delta);

    u_short nextPort(void);

    void startTimer(int timeout);

    void doRequest(NetRequest* request);

    void doWrite(NetRequest* request);

    void doRead(NetRequest* request);

    /*void reply(NetRequest* request, const boost::system::error_code& ec);
    void reply(NetResponse* response, const boost::system::error_code& ec);*/

private:
    bool                         threadStarted;
    int                          timeoutForConnect;
    int                          timeoutForRead;
    // xxsocket                     sock;

    tcp_client*                  client;
    std::list<NetRequest*>     requestq;
    std::list<NetResponse*>    responseq;
    bool                         needQuit;


    std::mutex                   requestqMtx;
    std::mutex                   responseqMtx;

    std::mutex                   sleepMutex;
    std::condition_variable      sleepCondition;

    unsigned long                asyncRequestCount;

    // exit safety
    std::mutex                   threadRunningMtx;
    std::condition_variable      threadRunningCond;

    std::string                  addr;
    std::vector<u_short>         ports;
    size_t                       portIndex;
};

};

// end of Network group
/// @}

#define network singleton<ins::NetClient>::instance()

#endif //__CCHTTPREQUEST_H__
