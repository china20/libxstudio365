
// INSPayment ios impl use storekit
#ifndef  _INS_PAYMENT_IMPL_IOS_H_
#define  _INS_PAYMENT_IMPL_IOS_H_

#include "INSPaymentTransaction.h" 
#include "../../../INSOLPayment.h"

static const int kProductsRequestErrorPreviousRequestNotCompleted = -2;
static const int kProductsRequestErrorCancelled = -1;

class INSProductsRequestDelegate
{
public:
	virtual void requestProductsCompleted(const void* products) = 0;
	virtual void requestProductsFailed(int errorCode, const char* errorString) = 0;

};

class INSPaymentTransactionObserver
{
public:
    virtual void transactionCompleted(INSPaymentTransaction* transaction) = 0;
    virtual void transactionFailed(INSPaymentTransaction* transaction) = 0;
    virtual void transactionRestored(INSPaymentTransaction* transaction) = 0;
};

class INSPaymentImpl_iOS : public inewsoft::ol::Payment,public INSPaymentTransactionObserver, public INSProductsRequestDelegate
{
public:
    
    ~INSPaymentImpl_iOS(void);
    
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
    void           finishTransaction(INSPaymentTransaction *transaction);
    
    /* delegates */
    virtual void transactionCompleted(INSPaymentTransaction* transaction);
    virtual void transactionFailed(INSPaymentTransaction* transaction);
    virtual void transactionRestored(INSPaymentTransaction* transaction);

	virtual void requestProductsCompleted(const void* products);
	virtual void requestProductsFailed(int errorCode, const char* errorString);;
	
private:
     void  doVerifyReceipt(INSPaymentTransaction* transaction);

private:
    INSPaymentImpl_iOS(void)
    {
    }
    bool init(void);
    inewsoft::ol::ReceiptVerifier                        receiptVerifier;
    inewsoft::ol::LoadProductCallback                    loadProductCallback;
    inewsoft::ol::TransactionCallback                    transactionCallback;
    std::map<const std::string, INSPaymentTransaction*>  transactions;
};

#endif // _INS_PAYMENT_IMPL_IOS_H_
