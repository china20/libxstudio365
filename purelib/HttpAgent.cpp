#include <functional>
#include "HttpAgent.h"
#include <time.h>
#include "purelib/utils/singleton.h"
#include "CCLuaEngine.h"

using namespace cocos2d;
//enum {
//	kRequestNumberGetNewUserId = 1,
//    kRequestNumberGetProductList = 20080,
//    kRequestNumberVerifyIosReceipt = 20081,
//};

const char* information_key = "ZQnNQmA1iIQ3z3ukoPoATdE88OJ0qsMm";

HttpAgent::HttpAgent(void)
{
    /*this->errorTable.insert(std::make_pair(response_number::cannot_invite_self, lang_utility::getStringByKey("Cannot Invite Self")));
    this->errorTable.insert(std::make_pair(response_number::exec_sql_failed, lang_utility::getStringByKey("Exec SQL Failed")));
    this->errorTable.insert(std::make_pair(response_number::feature_not_develop, lang_utility::getStringByKey("Feature Not Develop")));
    this->errorTable.insert(std::make_pair(response_number::invalid_request, lang_utility::getStringByKey("Invalid Request")));
    this->errorTable.insert(std::make_pair(response_number::invalid_user_id, lang_utility::getStringByKey("Invalid User Id")));
    this->errorTable.insert(std::make_pair(response_number::invited_user_not_exist, lang_utility::getStringByKey("Invited User Not Exist")));
    this->errorTable.insert(std::make_pair(response_number::password_incorrect, lang_utility::getStringByKey("Password Incorrect")));
    this->errorTable.insert(std::make_pair(response_number::request_expired, lang_utility::getStringByKey("Request Expired")));
    this->errorTable.insert(std::make_pair(response_number::session_expired, lang_utility::getStringByKey("Session Expired")));
    this->errorTable.insert(std::make_pair(response_number::username_conflict, lang_utility::getStringByKey("Username Conflict")));
    this->errorTable.insert(std::make_pair(response_number::username_incorrect, lang_utility::getStringByKey("Username Incorrect")));
    this->errorTable.insert(std::make_pair(response_number::unknown_error, lang_utility::getStringByKey("Network Error")));
    this->errorTable.insert(std::make_pair(response_number::donot_reinvite_friend, lang_utility::getStringByKey("Don't Reinvite Friend")));
    this->errorTable.insert(std::make_pair(response_number::you_are_already_friends, lang_utility::getStringByKey("You Are Already Friends")));
    this->errorTable.insert(std::make_pair(response_number::ios_verify_receipt_failed, lang_utility::getStringByKey("Verify Receipt Failed")));
    this->errorTable.insert(std::make_pair(response_number::peer_already_invite_you, lang_utility::getStringByKey("Peer Already Invite You")));
    this->errorTable.insert(std::make_pair(response_number::unexpected_data_received, lang_utility::getStringByKey("Data Unexpected")));
    this->errorTable.insert(std::make_pair(response_number::you_are_in_challenging, lang_utility::getStringByKey("You Are In Challenging")));
    this->errorTable.insert(std::make_pair(response_number::illegal_transaction, lang_utility::getStringByKey("Illegal Transaction")));*/
    auto L = ((LuaEngine*)ScriptEngineManager::getInstance()->getScriptEngine())->getLuaStack()->getLuaState();

    lua_settop(L, 0);
    int top = lua_gettop(L);
    lua_getglobal(L, "host_urls");

    int n = luaL_getn(L, -1);
    for (int i = 1; i <= 2; ++i)
    {
        lua_rawgeti(L, 1, i);
        
        httpclient->insertHostUrl(lua_tostring(L, -1));

        lua_pop(L, 1);
    }

    lua_settop(L, top);
}

const char* HttpAgent::strerr(int retval)
{
    auto target = this->errorTable.find(retval);
    if(target != this->errorTable.end())
    {
        return target->second.c_str();
    }
    return "Network Error!";
}

void HttpAgent::showErrorInfo(int retval)
{
    /*if(retval == response_number::session_expired && !localSettings->getUserName().empty() && localSettings->getSessionId() == 0)
    {
        createTextTagForWarnning("%s", lang_utility::getStringByKey("You Are Not Logged In, Please Login"));
    }
    else {
        createTextTagForWarnning("%s", this->strerr(retval), retval);
    }*/
}

void HttpAgent::post(const std::string& json, const HttpRespCallback& callback)
{
    std::string data = json;
    CCLOG("request plaintext:%s", data.c_str());

    crypto::aes::overlapped::encrypt(data, information_key);
    post_i(data, callback);
}

void HttpAgent::post(std::string&& data, const HttpRespCallback& callback)
{
    CCLOG("request plaintext:%s", data.c_str());

    crypto::aes::overlapped::encrypt(data, information_key);
    post_i(data, callback);
}

void HttpAgent::post_i(const std::string& data, const HttpRespCallback& callback)
{
    httpclient->send(data, [callback](NXHttpResp* response) {
        if (!response)
        {
            // response->getHttpRequest()->getUserData();
            return;
        }

        int statusCode = response->getResponseCode();
        char statusString[64] = {};
        sprintf(statusString, "HTTP Status Code: %d", statusCode);
        //m_labelStatusCode->setString(statusString);
        CCLOG("response code: %d", statusCode);

        if (!response->isSucceed())
        {
            CCLOG("response failed");

            //callback(response->getResponseCode(), jsonw::s_null_obj);
            return;
        }

        const std::vector<char>& resp = *response;
        auto plaintext = crypto::aes::decrypt(resp, information_key);
        

        response->push_back('\0');
        CCLOG("response ciphertext: size:%d\n", response->size() - 1);
        CCLOG("response plaintext: %s", plaintext.data());

        //jsonw::document parser(plaintext.data());
        //jsonw::object root(parser);
        //callback(root.value_of("ret", -1), root["params"]);
    });
}
