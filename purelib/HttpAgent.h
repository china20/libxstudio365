#ifndef _HTTPAGENT_H_
#define _HTTPAGENT_H_

#include "purelib/utils/json_wrapper.h"
#include "purelib/utils/crypto_wrapper.h"

#include "NXHttpClient.h"

// first: request type, second: return code
typedef uint32_t error_key;

#define MAKE_ERROR_KEY(request,ret) ( ( (uint32_t)(request) << 16 ) | ( (uint32_t)(ret) & 0xffff ) )

typedef std::function<void(int ret, const jsonw::object& params)> HttpRespCallback;

extern const char* information_key;

class HttpAgent
{
public:
    HttpAgent(void);

    // ~HttpAgent(void);

    const char* strerr(int retval);

    void showErrorInfo(int retval);

    // post request to server
    //format: {request_type:'',time:133434545,params: {...} }
    /*
     request json construct demo:
     jsonw::object root;
     root["udpate"] = 20150421;
     root["password"] = "265559666";
     root["peoples"]["guoxiaodong"]["age"] = 19;
    */
    void post(std::string&& json, const HttpRespCallback& callback);
    void post(const std::string& json, const HttpRespCallback& callback);
    
private:
    void post_i(const std::string& data, const HttpRespCallback& callback);

private:
    std::unordered_map<error_key, std::string> errorTable;
};

#define httpagent purelib::gc::singleton<HttpAgent>::instance()

#endif

