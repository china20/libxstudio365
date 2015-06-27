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
#ifndef _ONLINE_PAYMENT_ANDROID_H_
#define _ONLINE_PAYMENT_ANDROID_H_
#include <functional>
#include "ProtocolIAP.h"
#include "cocos2d.h"

class NxPayment_Android : public NxPayment, public cocos2d::plugin::PayResultListener
{
public:
    NxPayment_Android(const char* name);
    virtual ~NxPayment_Android();

    // bool init(void); // should call firstly

    // virtual void setupDeveloperInfo(cocos2d::plugin::TIAPDeveloperInfo& devInfo) = 0;

    virtual void             payForProduct(const char* productId, const TransactionCallback& callback) 

    virtual void             onPayResult(cocos2d::plugin::PayResultCode ret, const char* msg, cocos2d::plugin::TProductInfo info);

    // cocos2d::plugin::ProtocolIAP* getPlugin(void);
    
protected:
    std::string                                   name_;
    cocos2d::plugin::ProtocolIAP*     plugin_;
    TransactionCallback                   callback_;
};

#endif // __IAPOL_MANAGER_H__
