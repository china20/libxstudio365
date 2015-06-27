
#import "NXPaymentImpl_objc.h"
#include <string>

@implementation NXPaymentImpl_objc

static NXPaymentImpl_objc *s_sharedStore;

#pragma mark -
#pragma mark init

+ (NXPaymentImpl_objc *)sharedStore
{
    if (!s_sharedStore)
    {
        s_sharedStore = [[NXPaymentImpl_objc alloc] init];
    }
    return s_sharedStore;
}

+ (void) purgeSharedStore
{
    if (s_sharedStore)
    {
        [s_sharedStore release];
    }
}

- (id)init
{
    self = [super init];
    if (self)
    {
        transactionObserver_ = nil;
    }
    return self;
}

- (void)dealloc
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];

    [productRequest_ release];
    [super dealloc];
    s_sharedStore = NULL;
}

- (void)registerTransactionObserver:(NXPaymentTransactionObserver *)observer andDelegate:(NXProductsRequestDelegate *)delegate
{
    transactionObserver_ = observer;
    productRequestDelegate_ = delegate;
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}

#pragma mark -
#pragma mark Making a Purchase

- (BOOL)canMakePurchases
{
    return [SKPaymentQueue canMakePayments];
}

- (void)purchase:(SKProduct *)product
{
    [[SKPaymentQueue defaultQueue] addPayment:[SKPayment paymentWithProduct:product]];
}

- (void)finishTransaction:(SKPaymentTransaction *)transaction
{
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

#pragma mark -
#pragma mark Retrieving Product Information

- (void) requestProductData:(NSSet *)productsId
{
    if(transactionObserver_ == nullptr || productRequestDelegate_ == nullptr)
        return;
    
    if (productRequest_ != nullptr)
    {
        return;
    }
    
    SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:productsId];
    request.delegate = self;
    [request autorelease];
    [request start];
    productRequest_ = request;
}

- (void)cancelRequestProductData
{
    if (productRequest_ != nil)
    {
        [productRequest_ cancel];
        productRequest_ = nil;
        productRequestDelegate_->requestProductsFailed(kProductsRequestErrorCancelled,
                                                       "kProductsRequestErrorCancelled");
    }
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    productRequestDelegate_->requestProductsCompleted(response.products);
    // productRequestDelegate_->requestProductsCompleted(iProducts, invalidIdentifiers);
    request.delegate = nil;
    productRequest_ = nil;
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
    request.delegate = nil;
    productRequest_ = nil;
    productRequestDelegate_->requestProductsFailed(error.code, NSS2STR(error.localizedDescription));
}

#pragma mark -
#pragma mark Handling Transactions

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (int i = 0; i < [transactions count]; ++i)
    {
        SKPaymentTransaction *transaction = [transactions objectAtIndex:i];
        if (transaction.transactionState != SKPaymentTransactionStatePurchasing)
        {
            CCLOG("[INSPaymentImpl_obj] paymentQueueUpdatedTransactions() tid: %s",
                  NSS2STR(transaction.transactionIdentifier));
        }
        
        /**
         enum {
             SKPaymentTransactionStatePurchasing,
             SKPaymentTransactionStatePurchased,
             SKPaymentTransactionStateFailed,
             SKPaymentTransactionStateRestored
         };
         typedef NSInteger SKPaymentTransactionState;
         
         SKPaymentTransactionStatePurchasing:
         The transaction is being processed by the App Store.
         
         SKPaymentTransactionStatePurchased:
         The App Store successfully processed payment. Your application should provide
         the content the user purchased.
         
         SKPaymentTransactionStateFailed:
         The transaction failed. Check the error property to determine what happened.
         
         SKPaymentTransactionStateRestored:
         This transaction restores content previously purchased by the user.
         Read the originalTransaction property to obtain information about the original purchase.
         */
        
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                CCLOG("transaction '%s' SKPaymentTransactionStatePurchased",
                      [transaction.transactionIdentifier cStringUsingEncoding:NSUTF8StringEncoding]);
                
                    [self transactionCompleted:transaction];
                
                break;
            case SKPaymentTransactionStateFailed:
                CCLOG("transaction '%s' SKPaymentTransactionStateFailed",
                      [transaction.transactionIdentifier cStringUsingEncoding:NSUTF8StringEncoding]);
                CCLOG("error: %s",
                       [[transaction.error localizedDescription] cStringUsingEncoding:NSUTF8StringEncoding]);
                [self transactionFailed:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                CCLOG("transaction '%s' SKPaymentTransactionStateRestored",
                      [transaction.transactionIdentifier cStringUsingEncoding:NSUTF8StringEncoding]);
                [self transactionRestored:transaction];
                break;
        }
    }
}
- (void)transactionCompleted:(SKPaymentTransaction *)transaction
{
    int error_code = transaction.error.code;
    CCLOG("transaction success, error code: %d", error_code);
    transactionObserver_->transactionCompleted([self createPaymentTransaction:transaction
                                                              ]);
}

- (void)transactionFailed:(SKPaymentTransaction *)transaction
{
    int error_code = transaction.error.code;
    CCLOG("transaction failed, error code: %d.", error_code);
    transactionObserver_->transactionFailed([self createPaymentTransaction:transaction]);
}

- (void)transactionRestored:(SKPaymentTransaction *)transaction;
{
    int error_code = transaction.error.code;
    CCLOG("transaction restored, error code: %d.", error_code);
    transactionObserver_->transactionRestored([self createPaymentTransaction:transaction]);
}


#pragma mark -
#pragma mark helper

- (NXPaymentTransaction *)createPaymentTransaction:(SKPaymentTransaction *)transaction
{
    const char *ccid        = NSS2STR(transaction.transactionIdentifier);
    const char *ccproductId = NSS2STR(transaction.payment.productIdentifier);
    int quantity            = transaction.payment.quantity;
    double dateTime         = [transaction.transactionDate timeIntervalSince1970];
    int receiptDataLength   = 0;
    const void *receiptData = NULL;
    int errorCode           = 0;
    const char *errorDescription = NULL;
    
    INSPaymentTransactionState ccstate = kPaymentTransactionStateNull;
    switch (transaction.transactionState)
    {
        case SKPaymentTransactionStateFailed:
            errorCode = transaction.error.code;
            /**
             enum {
                 SKErrorUnknown,
                 SKErrorClientInvalid,       // client is not allowed to issue the request, etc.
                 SKErrorPaymentCancelled,    // user cancelled the request, etc.
                 SKErrorPaymentInvalid,      // purchase identifier was invalid, etc.
                 SKErrorPaymentNotAllowed    // this device is not allowed to make the payment
             };
             */
            if (errorCode == SKErrorPaymentCancelled)
            {
                ccstate = kPaymentTransactionStateCancelled;
            }
            else
            {
                ccstate = kPaymentTransactionStateFailed;
            }
            errorDescription = NSS2STR(transaction.error.localizedDescription);
            break;
        case SKPaymentTransactionStatePurchased:
            ccstate = kPaymentTransactionStatePurchased;
            receiptDataLength = transaction.transactionReceipt.length;
            receiptData = transaction.transactionReceipt.bytes;
            break;
        case SKPaymentTransactionStatePurchasing:
            ccstate = kPaymentTransactionStatePurchasing;
            break;
        case SKPaymentTransactionStateRestored:
            ccstate = kPaymentTransactionStateRestored;
    }
    
    if (transaction.originalTransaction)
    {
        NXPaymentTransaction* originalTransaction = [self createPaymentTransaction:transaction.originalTransaction];
        return NXPaymentTransaction::transactionWithState(transaction
,
                                                               ccstate,
                                                               ccid,
                                                               ccproductId,
                                                               quantity,
                                                               dateTime,
                                                               receiptDataLength,
                                                               receiptData,
                                                               errorCode,
                                                               errorDescription,
                                                               originalTransaction);
    }
    else
    {
        return NXPaymentTransaction::transactionWithState(transaction
,
                                                               ccstate,
                                                               ccid,
                                                               ccproductId,
                                                               quantity,
                                                               dateTime,
                                                               receiptDataLength,
                                                               receiptData,
                                                               errorCode,
                                                               errorDescription,
                                                               NULL);
    }
}

@end
