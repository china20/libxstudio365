/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "NxPayment_Android.h"
#include "PluginManager.h"
#include "cocos2d.h"

using namespace cocos2d::plugin;
using namespace cocos2d;

static std::string getTransactionId(void)
{   
#define NUMBER_FORMAT "%d%02d%02d%02d%02d%02d%03d%d"
    char tms[128];
#ifdef _WIN32
    SYSTEMTIME t;
    GetLocalTime(&t);
    sprintf_s(tms, sizeof(tms), NUMBER_FORMAT, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds, rrand(10000, 99999));
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* t = localtime(&tv.tv_sec);
    sprintf(tms, NUMBER_FORMAT, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, (int)tv.tv_usec / 1000, rrand(10000, 99999));
#endif
    return tms;
}

NxPayment_Android::NxPayment_Android(const char* name)
    : plugin(nullptr),
      name(name)
{

}

NxPayment_Android::~NxPayment_Android()
{
    PluginManager::getInstance()->unloadPlugin(this->name.c_str());
    this->plugin = NULL;
}

bool NxPayment_Android::init(void)
{
    if(this->plugin != nullptr)
        return true;

    this->plugin = dynamic_cast<ProtocolIAP*>(PluginManager::getInstance()->loadPlugin(this->name.c_str()));

    cocos2d::plugin::Argument devInfo;
    
    // For Alipay
    /*devInfo["AlipayPartner"] = "2088111266215513";
    devInfo["AlipaySeller"] = "yiyou@qilaiwan.com";
    devInfo["AlipayRsaPrivate"] = "MIICXAIBAAKBgQC9hZyBktXJGlFREJ6M8vJqtAe3hnpr04Nh2l4GMk3pZvijTTojyY7mMhMm5OsD/R8bW7joC7WhRik12wLRwOJjG/sNtyjg0hk05BzRnetxZp0oFggppH/HmCSGYBNm085OPROsk6PDzA/Sq45d531yAZd3s/PrQnYKq4aTmrXylQIDAQABAoGAP20w1Jh8ncIGBthGB6vi/1zi3EPQQrfV13DKWM6LDprciWJ2G7X/8gA+Mp0YHgyfVXub5WTN03x0nEaAqgwgdzFAN3c+rv/iqNNk9JudD5voY6+rZoMDghAjKhk3iiZ2EgX+YETUFaroznDhWVR0k6dwCmw9PXq6Fb643N3gTyECQQDlL7uIjLTWgZPHMhY2kjBgi8OA4FW/JVl3mV+xhfcoGgbQSIw7U5CXvfsbuM7h/0AZmXgl/6Rj1vw3W6uNmzrZAkEA07HnJTBm90oSqBtkaASPXa1kiQcHDI8Xy00KyBJOETlSSnA48mzQUe0BGxgfAifnOqr6Q4y6GFgP+S9to6WIHQJAKLrt6huPe9u1Zp45ImOio1XTXdEAjCLYHpAsWIFFZmQRt+xct6JnPQBvYwLaCYHyY1pJ5v7iuTeYxUHOYDEpKQJAdQdWZyK46WBTrAdonHBY6Uqf13jBFtpMJyGyIiSsb60mpuwfLzWkfIXvJJFTIxf4JmC69Xjor+iO/AySKfOqqQJBAIQYV0/dpvY9VOGJVLW7PVD4V+XRuvWYN8w2zW8VwwPUgqFEqngGZ/9jwTFbgcYECHEKBGwCcXSoSwlFlpsNLdg=";
    devInfo["AlipayPublic"] = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCnxj/9qwVfgoUh/y2W89L6BkRAFljhNhgPdyPuBV64bfQNN1PjbCzkIM6qRdKBoLPXmKKMiFYnkd6rAoprih3/PrQEB/VsW8OoM8fxn67UDYuyBTqA23MML9q1+ilIZwBC2AQ2UBVOrFXfFl75p6/B5KsiNG9zpgmLCUYuLkxpLQIDAQAB";
    devInfo["AlipayPluginName"] = "alipay_msp.apk";
    devInfo["AlipayRoyPayAccount"] = "";
    devInfo["AlipayRoyReceiveAccount"] = "";
    devInfo["AlipayRoyPercent"] = "0.0";
    devInfo["AlipayRoyTip"] = "";
    devInfo["AlipayNotifyUrl"] = "http://42.121.35.53/alipay,notify";*/
    
    this->plugin->setDebugMode(true);
    this->plugin->configDeveloperInfo(devInfo);
    this->plugin->setResultListener(this);
    
    return true;
}

void NxPayment_Android::payForProduct(const char* productId, const TransactionCallback& callback)
{
    this->callback = callback;
    auto target = this->productTable.find(productId);
    if(target == this->productTable.end())
    	 callback(false);
        return;

    cocos2d::plugin::Argument args;
    args["productName"] = productId;
    args["productPrice"] = nsc::to_string(target->second.price);
    args["productDesc"] = target->second.desc;
    args["transactionId"] = getTransactionId();
    
    plugin->payForProduct(info);
}

void pay::onPayResult(PayResultCode ret, const char* msg, TProductInfo info)
{
    this->callback(ret, msg, info);
}



