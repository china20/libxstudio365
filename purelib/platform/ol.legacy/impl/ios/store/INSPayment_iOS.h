
#ifndef _INS_PAYMENT_IOS_H_
#define _INS_PAYMENT_IOS_H_
#include <functional>
#include <map>
#include <unordered_map>
#include <string>
#include "cocos2d.h"
#include "INSStoreProductsRequestDelegate.h"
#include "INSStoreTransactionObserver.h"
//#include "thelib/platform/CCNative.h"
#include "thelib/utils/singleton.h"
#include "thelib/INSCCSingleton.h"

#include "../../../INSOLPayment.h"

USING_NS_CC;
USING_NS_CC_EXT;

class INSPayment_iOS : public CCObject, public inewsoft::ol::Payment, public INSStoreTransactionObserver, public INSStoreProductsRequestDelegate
{
public:
    INSPayment_iOS(void);
    
    CREATE_FUNC(INSPayment_iOS);
    
    bool init(void);
    
    virtual void  loadProducts(inewsoft::ol::LoadProductCallback callback);
    virtual void  payForProduct(const char* productId, const inewsoft::ol::TransactionCallback& callback);
        
#pragma mark -
#pragma mark INSStoreTransactionObserver
    
    virtual void transactionCompleted(INSStorePaymentTransaction* transaction);
    virtual void transactionFailed(INSStorePaymentTransaction* transaction);
    virtual void transactionRestored(INSStorePaymentTransaction* transaction);
    
    void  doVerifyReceipt(INSStorePaymentTransaction* transaction);

#pragma mark -
#pragma mark INSStoreProductsRequestDelegate
    
    virtual void requestProductsCompleted(CCArray* products, CCArray* invalidProductsId = NULL);
    virtual void requestProductsFailed(int errorCode, const char* errorString);

    
#pragma mark -
#pragma mark helper
    
private:
    inewsoft::ol::LoadProductCallback loadProductCallback;
    inewsoft::ol::TransactionCallback transactionCallback;
    
    inewsoft::ol::ProductsState  productsState;
    bool           bIsPurchasePending;
    std::string    pendingProductId;
};


#endif
