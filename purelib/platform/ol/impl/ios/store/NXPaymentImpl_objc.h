
#import <StoreKit/StoreKit.h>

#include "NXPaymentImpl_iOS.h"

USING_NS_CC_EXT;

@interface NXPaymentImpl_objc : NSObject < SKPaymentTransactionObserver, SKProductsRequestDelegate >
{
    NXPaymentTransactionObserver *transactionObserver_;
    NXProductsRequestDelegate *productRequestDelegate_;
    SKProductsRequest *productRequest_;
}

#pragma mark -
#pragma mark init

+ (NXPaymentImpl_objc *)sharedStore;
+ (void)purgeSharedStore;
- (id)init;
- (void)registerTransactionObserver:(NXPaymentTransactionObserver *)observer andDelegate:(NXProductsRequestDelegate *)delegate;

#pragma mark -
#pragma mark Making a Purchase
- (BOOL)canMakePurchases;
- (void)purchase:(SKProduct *)product;
- (void)finishTransaction:(SKPaymentTransaction *)transaction;

#pragma mark -
#pragma mark Retrieving Product Information

- (void)requestProductData:(NSSet *)productsId;
- (void)cancelRequestProductData;

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error;

#pragma mark -
#pragma mark Handling Transactions

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;

- (void)transactionCompleted:(SKPaymentTransaction *)transaction;
- (void)transactionFailed:(SKPaymentTransaction *)transaction;
- (void)transactionRestored:(SKPaymentTransaction *)transaction;

#pragma mark -
#pragma mark helper

- (NXPaymentTransaction *)createPaymentTransaction:(SKPaymentTransaction *)transaction;

@end
