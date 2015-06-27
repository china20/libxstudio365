
#ifndef _NX_PAYMENT_IOS_H_
#define _NX_PAYMENT_IOS_H_
#include <functional>
#include <map>
#include <unordered_map>
#include <string>
#include "cocos2d.h"
#include "NXStoreProductsRequestDelegate.h"
#include "NXStoreTransactionObserver.h"
//#include "thelib/platform/CCNative.h"
#include "thelib/utils/singleton.h"
#include "thelib/NXSingleton.h"

#include "../../../NXPayment.h"

USING_NS_CC;
USING_NS_CC_EXT;

class NXPayment_iOS : public CCObject, public NXPayment, public NXStoreTransactionObserver, public NXStoreProductsRequestDelegate
{
public:
    NXPayment_iOS(void);
    
    CREATE_FUNC(NXPayment_iOS);
    
    bool init(void);
    
    virtual void  loadProducts(ol::LoadProductCallback callback);
    virtual void  payForProduct(const char* productId, const ol::TransactionCallback& callback);
        
#pragma mark -
#pragma mark NXStoreTransactionObserver
    
    virtual void transactionCompleted(NXStorePaymentTransaction* transaction);
    virtual void transactionFailed(NXStorePaymentTransaction* transaction);
    virtual void transactionRestored(NXStorePaymentTransaction* transaction);
    
    void  doVerifyReceipt(NXStorePaymentTransaction* transaction);

#pragma mark -
#pragma mark NXStoreProductsRequestDelegate
    
    virtual void requestProductsCompleted(CCArray* products, CCArray* invalidProductsId = NULL);
    virtual void requestProductsFailed(int errorCode, const char* errorString);

    
#pragma mark -
#pragma mark helper
    
private:
    ol::LoadProductCallback loadProductCallback;
    ol::TransactionCallback transactionCallback;
    
    ol::ProductsState  productsState;
    bool           bIsPurchasePending;
    std::string    pendingProductId;
};


#endif
