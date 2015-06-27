// Copyright (c) 2014 inewsoft Corporation. All rights reserved.
#ifndef _INSOLPAYMENTINTERFACE_H_
#define _INSOLPAYMENTINTERFACE_H_

#include <functional>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

namespace inewsoft {
namespace ol {

/// Load product callback
typedef std::function<void(bool succeed)> LoadProductCallback;

/// TrasactionCallback
typedef std::function<void(bool succeed, const std::string& transactionId, const char* error)> TransactionCallback;

/// VerifyCallback
typedef std::function<void(bool succeed)>
    ReceiptVerifyCallback;
    
/// Verifier
    typedef std::function<void(const char* receiptData, size_t receiptDataLen, const ReceiptVerifyCallback& callback)>
    ReceiptVerifier;

/// ProductsState
enum ProductState {
    kProductStateUnload,
    kProductStateLoading,
    kProductStateLoaded,
};
    
/// PaymentState
enum PaymentState {
    kPaymentStateIdle,
    kPaymentStatePurchasing,
    kPaymentStatePurchaseSucess,
    kPaymentStatePurchaseFailed,
};

/// Product info;
struct ProductInfo
{
    ProductInfo(void) : id(), name(), desc(), price(.0f), value(0), userData(nullptr){};
    std::string id;
    std::string name;  // Alipay
    std::string desc;  // Alipay
    float       price; // price $ RMB etc.
    int         value; // product count such as diamond count
    void*       userData;
};

extern ProductInfo ProductInfoEmpty;

/// Payment interface
class Payment
{
public:
    Payment(void) : productState(kProductStateUnload), paymentState(kPaymentStateIdle) {};
    virtual ~Payment(void) {};

    /*
     * in ios: load products id from developer server, request products from appstore server
     * other os: developer server only
     */
    virtual void             loadProducts(LoadProductCallback callback);

    virtual void             payForProduct(const char* productId, const TransactionCallback& callback) = 0;
    
    virtual void             setReceiptVerifier(const inewsoft::ol::ReceiptVerifier& verifier){};

    std::vector<ProductInfo> getProductList(void);

protected:
    
    std::unordered_map<std::string, ProductInfo> productTable; // product id <--> product info
    ProductState                                 productState;
    PaymentState                                 paymentState;
};

extern Payment* getPayment(void);

};
};

#define  appstore ins::ol::getPayment()


#endif


