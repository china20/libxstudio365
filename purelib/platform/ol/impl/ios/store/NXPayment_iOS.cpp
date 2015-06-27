
#include "NXPayment_iOS.h"
#include <base/ccCArray.h>
#include "NXStore.h"
#include "thelib/utils/nsconv.h"
#include "thelib/utils/crypto_wrapper.h"
//#include "LocalSettings.h"
//#include "UIGameMainScene.h"
//#include "HttpAgent.h"

static CCString* newCCString(const char* string)
{
    CCString* str = new CCString(string);
    str->autorelease();
    return str;
}

NXPayment_iOS::NXPayment_iOS(void) : productsState(inewsoft::ol::kProductsStateUnload), bIsPurchasePending(false)
{
}

bool NXPayment_iOS::init(void)
{
    // loadProducts([](bool){}); // do nothing
    return true;
}

void NXPayment_iOS::loadProducts(inewsoft::ol::LoadProductCallback callback)
{
    if(!this->productTable.empty() && productsState == inewsoft::ol::kProductsStateLoaded)
    {
        callback(true);
        return;
    }
    else if(this->productsState == inewsoft::ol::kProductsStateLoading)
    {
        this->loadProductCallback = callback;
        return;
    }
    
    /// loald product from developer server firstly
    this->productsState = inewsoft::ol::kProductsStateLoading;
    Payment::loadProducts([this, callback](bool succeed){
        if(succeed) {
            NXStore::sharedStore()->registerTransactionObserver(this);
            CCArray* productsId = new CCArray();
            productsId->autorelease();
            
            for(auto iter = this->productTable.begin(); iter != this->productTable.end(); ++iter)
            {
                productsId->addObject(newCCString(iter->second.id.c_str()));
            }
            
            this->loadProductCallback = callback;
            NXStore::sharedStore()->loadProducts(productsId, this);
        }
        else {
            this->productsState = inewsoft::ol::kProductsStateUnload;
            callback(false);
            
        }
    });
}

void  NXPayment_iOS::payForProduct(const char* productId, const inewsoft::ol::TransactionCallback& callback)
{
    
    if(this->transactionCallback != nullptr || this->bIsPurchasePending)
    { // please wait...
        //CCNative::createAlert("请稍后", "请等待上次交易完成。。。", "OK");
        //CCNative::showAlert();
        return;
    }
    
    this->transactionCallback = callback;
    switch (this->productsState)
    {
        case kProductsStateLoaded:
            NXStore::sharedStore()->purchase(productId);
            break;
            
        case kProductsStateUnload:
            this->bIsPurchasePending = true;
            this->pendingProductId = productId;
            this->loadProducts(nullptr);
            break;
            
        default:
            break;
    }
}

#pragma mark -
#pragma mark CCStoreTransactionObserver

void NXPayment_iOS::transactionCompleted(NXStorePaymentTransaction* transaction)
{
    CCLOG("TransactionIdentifier: %s\n", transaction->getTransactionIdentifier().c_str());
    
    this->doVerifyReceipt(transaction);

    this->bIsPurchasePending = false;
}

void NXPayment_iOS::transactionRestored(NXStorePaymentTransaction* transaction)
{
    CCLOG("TransactionIdentifier: %s\n", transaction->getTransactionIdentifier().c_str());
    CCLOG("OriginTransactionIdentifier: %s\n",
          transaction->getOriginalTransaction()->getTransactionIdentifier().c_str());
    
    this->doVerifyReceipt(transaction);

    this->bIsPurchasePending = false;
}

void NXPayment_iOS::transactionFailed(NXStorePaymentTransaction* transaction)
{
    NXStore::sharedStore()->finishTransaction(transaction);
    
    if(this->transactionCallback == nullptr)
    {
        return;
    }
    auto target = this->productTable.find(transaction->getProductIdentifier());
    if(target != this->productTable.end())
    {
        this->transactionCallback(false, transaction->getTransactionIdentifier(), transaction->getErrorDescription().c_str());
    }
    else {
        this->transactionCallback(false, "unexcept error occured", "");
    }
    
    this->bIsPurchasePending = false;
    this->transactionCallback = nullptr;
}

void  NXPayment_iOS::doVerifyReceipt(NXStorePaymentTransaction* transaction)
{
    auto target = this->productTable.find(transaction->getProductIdentifier());
    if( this->productTable.end() == target )
    {
        NXStore::sharedStore()->finishTransaction(transaction);
        if(transactionCallback)
            this->transactionCallback(false, transaction->getTransactionIdentifier(), "");
        this->transactionCallback = nullptr;
        return;
    }
    
    this->receiptData.assign((const char*)transaction->getReceiptData(), transaction->getReceiptDataLength());
    
    transaction->retain();
    
    /*httpagent->post(httpagent->makeJsonForVerifyReceipt(this->receiptData), [this,transaction](int ret, const cjsonw::object& params){
        
        if(ret == 0) { // purchase success
            CCStore::sharedStore()->finishTransaction(transaction);
            if(transactionCallback != nullptr) {
                this->transactionCallback(true, transaction->getTransactionIdentifier(), "");
            }
            else {
                // complete last uncomplete transaction
                int plus = this->productTable[transaction->getProductIdentifier()].value;
                localSettings->setDiamond(
                                          localSettings->getDiamond() + plus);
                
                createTextTagForNormal("%s,%s+%d", lang_utility::getStringByKey("Transaction Restore Success"), lang_utility::getStringByKey("Diamond"), plus);
                
                mainui->updateRichLabels(kRichTypeDiamond);
            }
        }
        else if(ret == (int)ios_verify_receipt_failed)
        {
            CCStore::sharedStore()->finishTransaction(transaction);
            if(transactionCallback)
                this->transactionCallback(false, transaction->getTransactionIdentifier(), httpagent->strerr(ret));
        }
        else if(ret < 0) {
            if(transactionCallback)
                this->transactionCallback(false, transaction->getTransactionIdentifier(), httpagent->strerr(ret));
        }
        
        transaction->release();
        
        this->transactionCallback = nullptr;
    });*/
}

#pragma mark -
#pragma mark CCStoreProductsRequestDelegate

void NXPayment_iOS::requestProductsCompleted(CCArray* products, CCArray* invalidProductsId)
{
    for (int i = 0; i < products->count(); ++i)
    {
        INSStoreProduct* product = static_cast<INSStoreProduct*>(products->objectAtIndex(i));
        CCLOG("PRODUCT ID: %s\n",              product->getProductIdentifier().c_str());
        CCLOG("  localizedTitle: %s\n",        product->getLocalizedTitle().c_str());
        CCLOG("  localizedDescription: %s\n",  product->getLocalizedDescription().c_str());
        CCLOG("  priceLocale: %s\n",           product->getPriceLocale().c_str());
        CCLOG("  price: %0.2f\n",              product->getPrice());
        
        auto& pi = this->productTable[product->getProductIdentifier()];
        pi.price = product->getPrice();
        pi.value = thelib::nsc::to_numeric<int>(product->getLocalizedDescription());
    }
    //printf("\n");
    
    if (invalidProductsId && invalidProductsId->count() > 0)
    {
        printf("FOUND INVALID PRODUCTS ID\n");
        for (int i = 0; i < invalidProductsId->count(); ++i)
        {
            CCString* ccid = static_cast<CCString*>(invalidProductsId->objectAtIndex(i));
            CCLOG("  %s\n", ccid->getCString());
        }
    }
    
    this->productsState = kProductsStateLoaded;
    
    if(this->loadProductCallback != nullptr)
    {
        this->loadProductCallback(true);
        this->loadProductCallback = nullptr;
    }
    
    if(this->bIsPurchasePending)
    {
        this->payForProduct(this->pendingProductId.c_str(), this->transactionCallback);
    }
}

void NXPayment_iOS::requestProductsFailed(int errorCode, const char* errorString)
{
    this->productsState = kProductsStateUnload;
    
    if(this->loadProductCallback != nullptr)
    {
        this->loadProductCallback(false);
        this->loadProductCallback = nullptr;
    }
    
    this->bIsPurchasePending = false;
}


