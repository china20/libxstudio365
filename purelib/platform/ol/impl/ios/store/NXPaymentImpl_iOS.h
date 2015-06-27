
// NXPayment ios impl use storekit
#ifndef  _NX_PAYMENT_IMPL_IOS_H_
#define  _NX_PAYMENT_IMPL_IOS_H_

#include "NXPaymentTransaction.h" 
#include "../../../NXOLPayment.h"

static const int kProductsRequestErrorPreviousRequestNotCompleted = -2;
static const int kProductsRequestErrorCancelled = -1;

class NXProductsRequestDelegate
{
public:
	virtual void requestProductsCompleted(const void* products) = 0;
	virtual void requestProductsFailed(int errorCode, const char* errorString) = 0;

};

class NXPaymentTransactionObserver
{
public:
    virtual void transactionCompleted(NXPaymentTransaction* transaction) = 0;
    virtual void transactionFailed(NXPaymentTransaction* transaction) = 0;
    virtual void transactionRestored(NXPaymentTransaction* transaction) = 0;
};

class NXPaymentImpl_iOS : public NXPayment,public NXPaymentTransactionObserver, public NXProductsRequestDelegate
{
public:
    
    ~NXPaymentImpl_iOS(void);
    
    // 确认当前是否可以进行交易
    bool          canMakePurchases(void);
   
    bool          isProductsLoading(void);
    
    // 载入指定产品的信息
    virtual void  loadProducts(inewsoft::ol::LoadProductCallback callback);
    
    virtual void  payForProduct(const char* productId,
                                const inewsoft::ol::TransactionCallback& callback);
    
    virtual void  setReceiptVerifier(const inewsoft::ol::ReceiptVerifier& verifier);

    // 取消载入产品信息的操作
    void           cancelLoadProducts(void);
    
    /** @brief 确定指定的产品是否已经载入
     
     @param productId 要检查的产品 ID
     */
    bool          isProductLoaded(const char* productId);
    
    /** @brief 购买指定的产品
     
     @param productId 要购买产品的 ID
     
     指定 ID 的产品信息必须之前成功调用 loadProducts() 载入。如果指定的产品尚未载入，则返回 false。
     */
    bool           purchase(const char* productId);
    
    /* @brief 结束交易 */
    void           finishTransaction(NXPaymentTransaction *transaction);
    
    /* delegates */
    virtual void transactionCompleted(NXPaymentTransaction* transaction);
    virtual void transactionFailed(NXPaymentTransaction* transaction);
    virtual void transactionRestored(NXPaymentTransaction* transaction);

	virtual void requestProductsCompleted(const void* products);
	virtual void requestProductsFailed(int errorCode, const char* errorString);;
	
private:
     void  doVerifyReceipt(NXPaymentTransaction* transaction);

private:
    NXPaymentImpl_iOS(void)
    {
    }
    bool init(void);
    ol::ReceiptVerifier                        receiptVerifier;
    ol::LoadProductCallback                    loadProductCallback;
    ol::TransactionCallback                    transactionCallback;
    std::map<const std::string, NXPaymentTransaction*>  transactions;
};

#endif // _INS_PAYMENT_IMPL_IOS_H_
