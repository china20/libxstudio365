#include "NXPaymentImpl_iOS.h"
#import "NXPaymentImpl_objc.h"

NXPaymentImpl_iOS::~NXPaymentImpl_iOS(void)
{
    for (auto it = transactions.begin(); it != transactions.end(); ++it)
    {
        it->second->release();
    }
}

bool NXPaymentImpl_iOS::init(void)
{
    [[NXPaymentImpl_objc sharedStore] registerTransactionObserver:this andDelegate:this];
    return true;
}

bool NXPaymentImpl_iOS::canMakePurchases(void)
{
    return [[NXPaymentImpl_objc sharedStore] canMakePurchases];
}

bool NXPaymentImpl_iOS::isProductsLoading()
{
    return false;// return  [[NXPaymentImpl_objc sharedStore] isProductsLoading];
}

void  NXPaymentImpl_iOS::payForProduct(const char* productId, const inewsoft::ol::TransactionCallback& callback)
{
    if (paymentState == inewsoft::ol::kPaymentStatePurchasing) {
        if(callback)
            callback(false, productId, "Wait Perviously Purchase Complete.");
        return;
    }
    
    this->transactionCallback = callback;
    switch (this->productState) {
        case inewsoft::ol::kProductStateLoaded:
            paymentState = inewsoft::ol::kPaymentStatePurchasing;
            this->purchase(productId);
            break;
            
        case inewsoft::ol::kProductStateLoading:
            if(callback)
                callback(false, productId, "Wait Products Loading");
            break;
            
        case inewsoft::ol::kProductStateUnload:
            {
                std::string copiedId(productId);
                this->loadProducts([copiedId,this](bool succeed) {
                    if(succeed) {
                        this->purchase(copiedId.c_str());
                    }
                    else {
                        if(this->transactionCallback)
                        {
                            transactionCallback(false, copiedId.c_str(), "Products Load Failed");
                        }
                    }
                });
            }
            break;
        default:
            assert(false);
    };
}

void NXPaymentImpl_iOS::loadProducts(inewsoft::ol::LoadProductCallback callback)
{
    switch (productState) {
        case inewsoft::ol::kProductStateLoading:
        case inewsoft::ol::kProductStateLoaded:
            return;
            
        default: ;
    }
    
    this->productState = inewsoft::ol::kProductStateLoading;
    
    this->loadProductCallback = callback;
    inewsoft::ol::Payment::loadProducts([this](bool succeed){
        if(succeed)
        {
            NSMutableSet* set = [NSMutableSet set];
            for (auto iter = this->productTable.begin(); iter != this->productTable.begin(); ++iter)
            {
                [set addObject:[NSString stringWithUTF8String: iter->second.id.c_str()]];
            }
            [[NXPaymentImpl_objc sharedStore] requestProductData:set];
        }
        else {
            CCLOG("%s", "request development server failed");
            this->productState = inewsoft::ol::kProductStateLoading;
            if(this->loadProductCallback)
            {
                this->loadProductCallback(false);
            }
        }
    });
}

void NXPaymentImpl_iOS::cancelLoadProducts(void)
{
    [[NXPaymentImpl_objc sharedStore] cancelRequestProductData];
}

bool NXPaymentImpl_iOS::isProductLoaded(const char* productId)
{
    return productState == inewsoft::ol::kProductStateLoaded;
}

bool NXPaymentImpl_iOS::purchase(const char* productId)
{
    if (!productId) return false;
    
    auto detail = productTable.find(productId);
    if(detail == productTable.end()) return false;
    
    SKProduct* product = (SKProduct*)detail->second.userData;
    
    if (product)
    {
        [[NXPaymentImpl_objc sharedStore] purchase:product];
        return true;
    }
    return false;
}

void NXPaymentImpl_iOS::finishTransaction(INSPaymentTransaction *transaction)
{
    
    [[NXPaymentImpl_objc sharedStore] finishTransaction:(SKPaymentTransaction *)transaction->getTransactionInternal()];
    //auto it = transactions.find(transaction->getTransactionIdentifier());
    //if (it != transactions.end())
   // {
    //    it->second->release();
     //   transactions.erase(it);
    //}
}


#pragma mark -
#pragma mark delegates

void NXPaymentImpl_iOS::transactionCompleted(INSPaymentTransaction* transaction)
{
    transactions[transaction->getTransactionIdentifier()] = transaction;
    
    // TODO: verify receipt
    doVerifyReceipt(transaction);
}

void NXPaymentImpl_iOS::transactionFailed(INSPaymentTransaction* transaction)
{
    transactions[transaction->getTransactionIdentifier()] = transaction;
    finishTransaction(transaction);
}

void NXPaymentImpl_iOS::transactionRestored(INSPaymentTransaction* transaction)
{
    transactions[transaction->getTransactionIdentifier()] = transaction;
    // TODO: verify receipt
}

void NXPaymentImpl_iOS::requestProductsCompleted(const void* productsOriginal)
{
    // TODO:
    const NSArray* products = (const NSArray*)productsOriginal;
    
    // update product info from itnues connect
    for (int i = 0; i < products.count; ++i)
    {
        // cache loaded product
        SKProduct* product = [products objectAtIndex:i];
        
        const char* productId = NSS2STR( product.productIdentifier );
        const char* productName = NSS2STR(product.localizedTitle);
        const char* productDesc = NSS2STR(product.localizedDescription);
        float productPrice  = [product.price floatValue];
        
        auto& pi = productTable[productId];
        pi.name = productName;
        pi.desc = productDesc;
        pi.price = productPrice;
        pi.userData = product;
        [product retain];
        // pi.value = xx inited by developer server.
    }
    
    
    if(products.count > 0)
    {
        this->productState = inewsoft::ol::kProductStateLoaded;
    }
    else {
        this->productState = inewsoft::ol::kProductStateUnload;
    }
    
    if(loadProductCallback)
        loadProductCallback(products.count > 0);
}

void NXPaymentImpl_iOS::requestProductsFailed(int errorCode, const char* errorString)
{
    CCLOG("%s", "request Products from itunes connect failed!");
    this->productState = inewsoft::ol::kProductStateUnload;
    if(loadProductCallback)
        loadProductCallback(false);
}

void  NXPaymentImpl_iOS::doVerifyReceipt(INSPaymentTransaction* transaction)
{
    auto target = this->productTable.find(transaction->getProductIdentifier());
    if( this->productTable.end() == target )
    {
        this->finishTransaction(transaction);
        if(transactionCallback)
            this->transactionCallback(false, transaction->getTransactionIdentifier(), "");
        this->transactionCallback = nullptr;
        return;
    }
    
    transaction->retain();
    
    // TODO: send receiptdata to developer server.
    this->receiptVerifier((const char*)transaction->getReceiptData(), transaction->getReceiptDataLength(), [this,transaction](bool succeed){
        
        if(succeed) { // purchase success
            finishTransaction(transaction);
            
            if(transactionCallback != nullptr) {
                this->transactionCallback(true, transaction->getTransactionIdentifier(), "");
            }
            else {
                // complete last uncompleted transaction
            }
        }
        else if(succeed == (int)20021 /* ios verify failed */)
        {
            this->finishTransaction(transaction);
            if(transactionCallback)
                this->transactionCallback(false, transaction->getTransactionIdentifier(), "");
        }
        else if(succeed < 0 /* network error */ ) {
            if(transactionCallback)
                this->transactionCallback(false, transaction->getTransactionIdentifier(), "");
        }
        
        transaction->release();
        
        this->transactionCallback = nullptr;
    });
}

