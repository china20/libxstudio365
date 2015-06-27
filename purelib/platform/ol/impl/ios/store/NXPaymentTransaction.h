
#ifndef _NX_PAYMENTTRANSACTION_H_
#define _NX_PAYMENTTRANSACTION_H_

#include <purelib/NXGameDefs.h>

typedef enum {
    kPaymentTransactionStateNull = 0,
    kPaymentTransactionStatePurchasing = 1,
    kPaymentTransactionStatePurchased,
    kPaymentTransactionStateFailed,
    kPaymentTransactionStateRestored,
    kPaymentTransactionStateCancelled,
} NXPaymentTransactionState;

static const char* const NXPaymentTransactionStateNames[] = {
    "unknown",
    "purchasing",
    "purchased",
    "failed",
    "restored",
    "cancelled"
};

class NXPaymentTransaction : public cocos2d::Ref
{
public:
    static NXPaymentTransaction* transactionWithState(void* transactionInternal,
                                                           NXPaymentTransactionState state,
                                                           const char* transactionId,
                                                           const char* productId,
                                                           int quantity,
                                                           double dateTime,
                                                           int receiptDataLength,
                                                           const void* receiptData,
                                                           int errorCode,
                                                           const char* errorDescription,
                                                           NXPaymentTransaction* originalTransaction);
    ~NXPaymentTransaction(void);
    
    void* getTransactionInternal(void)
    {
        return m_transactionInternal;
    }
    
    NXPaymentTransactionState getTransactionState(void)
    {
        return m_transactionState;
    }
    
    const std::string& getTransactionIdentifier(void)
    {
        return m_transactionIdentifier;
    }
    
    const std::string& getProductIdentifier(void)
    {
        return m_productIdentifier;
    }
    
    int getQuantity(void)
    {
        return m_quantity;
    }
    
    double getDateTime(void)
    {
        return m_dateTime;
    }
    
    int getReceiptDataLength(void)
    {
        return m_receiptDataLength;
    }
    
    const void* const getReceiptData(void)
    {
        return m_receiptData;
    }
    
    int getErrorCode(void)
    {
        return m_errorCode;
    }
    
    const std::string& getErrorDescription(void)
    {
        return m_errorDescription;
    }
    
    NXPaymentTransaction* getOriginalTransaction(void)
    {
        return m_originalTransaction;
    }
    
    
private:
    NXPaymentTransaction(void) {}
    
    bool initWithState(void* transactionInternal,
                       NXPaymentTransactionState state,
                       const char* transactionId,
                       const char* productId,
                       int quantity,
                       double dateTime,
                       int receiptDataLength,
                       const void* receiptData,
                       int errorCode,
                       const char* errorDescription,
                       NXPaymentTransaction* originalTransaction);
    
    void*                               m_transactionInternal; // Is SKPaymentTransaction
    NXPaymentTransactionState          m_transactionState;
    std::string                         m_transactionIdentifier;
    std::string                         m_productIdentifier;
    int                                 m_quantity;
    double                              m_dateTime;
    int                                 m_receiptDataLength;
    void*                               m_receiptData;
    int                                 m_errorCode;
    std::string                         m_errorDescription;
    NXPaymentTransaction*              m_originalTransaction;
};

#endif // _INS_PaymentTransaction_H_
