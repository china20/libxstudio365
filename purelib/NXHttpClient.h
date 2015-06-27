/****************************************************************************
Copyright (c) 2015 idreamsky.com
Copyright (c) 2013 purelib
****************************************************************************/

#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_
#include "ExtensionMacros.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <list>
#include <vector>
#include "curl/curl.h"

#include <cocos2d.h>

enum : unsigned char 
{
    kHttpRequestGet, 
    kHttpRequestPost
};

typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *userp);

class NXHttpResp;
typedef std::function<void(NXHttpResp* response)> HttpResponseCallback;
typedef std::function<void(float f)> HttpProgressCallback;

class NXHttpObject : public cocos2d::CCObject, public std::vector<char>
{
};

class NXHttpReq : public NXHttpObject
{
public:
    friend class NXHttpResp;
    /** Constructor
    Because HttpRequest object will be used between UI thead and network thread,
    requestObj->autorelease() is forbidden to avoid crashes in AutoreleasePool
    new/retain/release still works, which means you need to release it manually
    Please refer to HttpRequestTest.cpp to find its usage
    */
    NXHttpReq()
    {
    };

protected:
    /** Destructor */
    virtual ~NXHttpReq()
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
        this->assign(buffer, buffer + len);
        // this->copy(buffer, len);
    };
    /** Get the request data pointer back */
    inline char* getRequestData()
    {
        return this->data();
    }
    /** Get the size of request data back */
    inline int getRequestDataSize()
    {
        return this->size();
    }

    void setCallback(const HttpResponseCallback& responseCallback, const HttpProgressCallback& progressCallback = [](float p)->void{})
    {
        this->response_allback_ = responseCallback;
        this->progress_callback_ = progressCallback;
    }

    void notifyProgress(float p)
    {
        this->progress_callback_(p);
    }

protected:
    // properties
    int                         type_;    /// kHttpRequestGet, kHttpRequestPost or other enums
    // std::string                 _url;            /// target url that this request is sent to
    HttpResponseCallback        response_allback_;
    HttpProgressCallback        progress_callback_;
    //std::string                 _tag;            /// user defined tag, to identify different requests in response callback
};

class NXHttpResp : public NXHttpObject
{
public:
    NXHttpResp(NXHttpReq* request) : succeed_(false)
    {
        CC_ASSERT(request);
        request->retain();
        this->request_ = request;
    }
    virtual ~NXHttpResp(void)
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
        request_->response_allback_(this);
    }

protected:
    int                         responseCode_;
    bool                        succeed_;
    NXHttpReq*              request_;
};


class NXCurlAuto
{
    /// Instance of CURL
    CURL *curl_;
    /// Keeps custom header data
    curl_slist *headers_;
public:
    NXCurlAuto(void)
        : curl_(curl_easy_init())
        , headers_(NULL)
    {

    }

    ~NXCurlAuto(void)
    {
        if (curl_)
            curl_easy_cleanup(curl_);
        /* free the linked list for header data */
        if (headers_)
            curl_slist_free_all(headers_);
    }

    //Configure curl's timeout property
    bool configureCURL(const char* url, int timeoutForRead = 5, int timeOutForConnect = 30)
    {
        if (!this->curl_) {
            return false;
        }

        return setOption(CURLOPT_TIMEOUT, timeoutForRead) &&
            setOption(CURLOPT_CONNECTTIMEOUT, timeoutForRead) &&
            setOption(CURLOPT_SSL_VERIFYPEER, 0L) &&
            setOption(CURLOPT_SSL_VERIFYHOST, 0L) &&
            setOption(CURLOPT_URL, url);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(curl_, option, data);
    }

    /**
    * @brief Inits CURL instance for common usage
    * @param request Null not allowed
    * @param callback Response write callback
    * @param stream Response write stream
    */
    bool init(NXHttpObject *request, write_callback callback, void* userp)
    {
        if (!curl_)
            return false;

        return setOption(CURLOPT_WRITEFUNCTION, callback)
            && setOption(CURLOPT_WRITEDATA, userp);

    }

    /// @param responseCode Null not allowed
    bool perform(int *responseCode)
    {
        if (CURLE_OK != curl_easy_perform(curl_))
            return false;
        CURLcode code = curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || *responseCode != 200) {
            //CCLOG("Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
            return false;
        }
        // Get some mor data.

        return true;
    }
};

/** @brief Singleton that handles asynchrounous http requests
* Once the request completed, a callback will issued in main thread when it provided during make request
* Must call scheduleCollectResponse when start and unscheduleCollectResponse when application ending
*      
*/
class NXHttpClient : public cocos2d::CCObject
{
public:
    NXHttpClient();
    virtual ~NXHttpClient();

    bool init(void);
    void scheduleCollectResponse(void);
    void unscheduleCollectResponse(void);

    void insertHostUrl(const char* url);

    const std::string& getUrl(void) const;

    void               pollUrl(void);

    /** Enable cookie support. **/
    void enableCookies(const char* cookieFile);

    /**
    * Add a get request to task queue
    * @param request a HttpRequest object, which includes url, response callback etc.
    please make sure request->_requestData is clear before calling "send" here.
    */
    void send(const char* buffer, size_t len, const HttpResponseCallback& callback, const HttpProgressCallback& progressCallback = [](float p)->void{});
    void send(const std::string& message, const HttpResponseCallback& callback, const HttpProgressCallback& progressCallback = [](float p)->void{});


    /**
    * Change the connect timeout
    * @param value The desired timeout.
    */
    inline void setTimeoutForConnect(int value) {timeoutForConnect_ = value;};

    /**
    * Get connect timeout
    * @return int
    */
    inline int getTimeoutForConnect() {return timeoutForConnect_;}


    /**
    * Change the download timeout
    * @param value
    */
    inline void setTimeoutForRead(int value) {timeoutForRead_ = value;};


    /**
    * Get download timeout
    * @return int
    */
    inline int getTimeoutForRead() {return timeoutForRead_;};

    void networkThread(void);
private:

    int  processPostTask(NXHttpReq* request, write_callback callback, void *stream, int32_t *responseCode);

    /**
    * Init pthread mutex, semaphore, and create new thread for http requests
    * @return bool
    */
    bool lazyInitThreadSemphore();
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks(float delta);

    u_short getNextPort(void);
private:
    bool                         thread_started_;

    int                          timeoutForConnect_;
    int                          timeoutForRead_;
    NXCurlAuto                   curl_;
    std::list<NXHttpReq*>        requestq_;
    std::list<NXHttpResp*>       responseq_;
    bool                         need_quit_;


    std::mutex                   requestqMtx_;
    std::mutex                   responseqMtx_;

    std::mutex                   sleepMutex;
    std::condition_variable      sleepCondition;

    unsigned long                asyncRequestCount;

    // exit safety
    std::mutex                   thread_running_mutex;
    std::condition_variable      thread_running_condition;

    std::vector<std::string>     urls_;
    size_t                       urlIdx_;

    bool                         bUrlChanged_;
};

// end of Network group
/// @}

#define httpclient purelib::gc::delayed<NXHttpClient>::instance(&NXHttpClient::init)

#endif //__CCHTTPREQUEST_H__
