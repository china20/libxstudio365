
#ifndef _INS_PaymentTransaction_H_
#define _INS_PaymentTransaction_H_

#include <thelib/INSDefs.h>

typedef enum {
    kPaymentTransactionStateNull = 0,
    kPaymentTransactionStatePurchasing = 1,
    kPaymentTransactionStatePurchased,
    kPaymentTransactionStateFailed,
    kPaymentTransactionStateRestored,
    kPaymentTransactionStateCancelled,
} INSPaymentTransactionState;

static const char* const INSPaymentTransactionStateNames[] = {
    "unknown",
    "purchasing",
    "purchased",
    "failed",
    "restored",
    "cancelled"
};

class INSPaymentTransaction : public cocos2d::Ref
{
public:
    static INSPaymentTransaction* transactionWithState(void* transactionInternal,
                                                           INSPaymentTransactionState state,
                                                           const char* transactionId,
                                                           const char* productId,
                                                           int quantity,
                                                           double dateTime,
                                                           int receiptDataLength,
                                                           const void* receiptData,
                                                           int errorCode,
                                                           const char* errorDescription,
                                                           INSPaymentTransaction* originalTransaction);
    ~INSPaymentTransaction(void);
    
    void* getTransactionInternal(void)
    {
        return m_transactionInternal;
    }
    
    INSPaymentTransactionState getTransactionState(void)
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
    
    INSPaymentTransaction* getOriginalTransaction(void)
    {
        return m_originalTransaction;
    }
    
    
private:
    INSPaymentTransaction(void) {}
    
    bool initWithState(void* transactionInternal,
                       INSPaymentTransactionState state,
                       const char* transactionId,
                       const char* productId,
                       int quantity,
                       double dateTime,
                       int receiptDataLength,
                       const void* receiptData,
                       int errorCode,
                       const char* errorDescription,
                       INSPaymentTransaction* originalTransaction);
    
    void*                               m_transactionInternal; // Is SKPaymentTransaction
    INSPaymentTransactionState          m_transactionState;
    std::string                         m_transactionIdentifier;
    std::string                         m_productIdentifier;
    int                                 m_quantity;
    double                              m_dateTime;
    int                                 m_receiptDataLength;
    void*                               m_receiptData;
    int                                 m_errorCode;
    std::string                         m_errorDescription;
    INSPaymentTransaction*              m_originalTransaction;
};

#endif // _INS_PaymentTransaction_H_
