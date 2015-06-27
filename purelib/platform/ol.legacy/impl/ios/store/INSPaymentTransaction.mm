#include "INSPaymentTransaction.h"
#import <StoreKit/StoreKit.h>

INSPaymentTransaction*
INSPaymentTransaction::transactionWithState(void* transactionInternal,
                                                INSPaymentTransactionState state,
                                                const char* transactionId,
                                                const char* productId,
                                                int quantity,
                                                double dateTime,
                                                int receiptDataLength,
                                                const void* receiptData,
                                                int errorCode,
                                                const char* errorDescription,
                                                INSPaymentTransaction* originalTransaction)
{
    INSPaymentTransaction* transaction = new INSPaymentTransaction();
    transaction->initWithState(transactionInternal,
                               state,
                               transactionId,
                               productId,
                               quantity,
                               dateTime,
                               receiptDataLength,
                               receiptData,
                               errorCode,
                               errorDescription,
                               originalTransaction);
    transaction->autorelease();
    return transaction;
}

INSPaymentTransaction::~INSPaymentTransaction(void)
{
    CC_SAFE_RELEASE(m_originalTransaction);
    if (m_receiptData)
    {
        free(m_receiptData);
    }
    if (m_transactionInternal)
    {
        [(SKPaymentTransaction*)m_transactionInternal release];
    }
    
    CCLOG("~~ INSPaymentTransaction::~INSPaymentTransaction()");
}

bool INSPaymentTransaction::initWithState(void* transactionInternal,
                                              INSPaymentTransactionState state,
                                              const char* transactionId,
                                              const char* productId,
                                              int quantity,
                                              double dateTime,
                                              int receiptDataLength,
                                              const void* receiptData,
                                              int errorCode,
                                              const char* errorDescription,
                                              INSPaymentTransaction* originalTransaction)
{
    m_transactionInternal = transactionInternal;
    [(SKPaymentTransaction*)m_transactionInternal retain];
    m_transactionState = state;
    m_transactionIdentifier = transactionId;
    m_productIdentifier = productId;
    m_quantity = quantity;
    m_dateTime = dateTime;
    m_receiptDataLength = receiptDataLength;
    if (receiptDataLength > 0)
    {
        m_receiptData = malloc(receiptDataLength + 1);
        memset(m_receiptData, 0, receiptDataLength + 1);
        memcpy(m_receiptData, receiptData, receiptDataLength);
    }
    else
    {
        m_receiptData = NULL;
    }
    m_errorCode = errorCode;
    m_errorDescription = errorDescription ? errorDescription : "";
    m_originalTransaction = originalTransaction;
    if (m_originalTransaction)
    {
        m_originalTransaction->retain();
    }
    
    return true;
}



