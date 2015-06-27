
#import <StoreKit/StoreKit.h>

#include "INSPaymentImpl_iOS.h"

USING_NS_CC_EXT;

@interface INSPaymentImpl_objc : NSObject < SKPaymentTransactionObserver, SKProductsRequestDelegate >
{
    INSPaymentTransactionObserver *transactionObserver_;
    INSProductsRequestDelegate *productRequestDelegate_;
    SKProductsRequest *productRequest_;
}

#pragma mark -
#pragma mark init

+ (INSPaymentImpl_objc *)sharedStore;
+ (void)purgeSharedStore;
- (id)init;
- (void)registerTransactionObserver:(INSPaymentTransactionObserver *)observer andDelegate:(INSProductsRequestDelegate *)delegate;

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

- (INSPaymentTransaction *)createPaymentTransaction:(SKPaymentTransaction *)transaction;

@end
