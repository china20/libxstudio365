
#ifndef _NXPAYMENT_WIN32_H_
#define _NXPAYMENT_WIN32_H_
#include <functional>
#include <map>
#include <unordered_map>
#include <string>
#include "cocos2d.h"
#include "NxPayment.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class NxPayment_Win32 : public CCObject, public NxPayment
{
public:
    NxPayment_Win32(void);

    CREATE_FUNC(NxPayment_Win32);

    bool init(void);

    virtual void  payForProduct(const char* productId, const TransactionCallback& callback);


private:
    bool bIsProductsLoaded;
    bool bIsPurchasePending;
    bool bIsPurchaseSent;
};


#endif
