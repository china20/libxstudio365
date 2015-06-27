#include "NXOLPayment.h"
#include "purelib/NXSingleton.h"
#include <algorithm>
#include <cocos2d.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "NXPaymentImpl_iOS.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//#include "INSPayment_Win32.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
/// android default use alipay
#if defined(USE_GOOGLEPAY)
#include "NXPayment_GooglePay.h"
#else
#include "NXPayment_Alipay.h"
#endif

#endif

ol::Payment* inewsoft::ol::getPayment(void)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    purelib::Singleton<NXPaymentImpl_iOS>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    return NULL;//
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if defined(USE_GOOGLEPAY)
    return purelib::Singleton<NXPayment_GooglePay>::instance();
#else
    return purelib::Singleton<NXPayment_Alipay>::instance();
#endif
#else
    return nullptr;
#endif
}

ol::ProductInfo ol::ProductInfoEmpty = ol::ProductInfo();

void Payment::loadProducts(inewsoft::ol::LoadProductCallback callback)
{
    /*NXHttpAgent::post(NXHttpAgent::makeJsonForGetProductList(), [this,callback](int ret, const cjsonw::object& params){

        if(ret == response_number::success) {
            params.getObject("product_list").cforeach([this](const cjsonw::object& product){
                auto result = this->productTable.insert(std::make_pair(product.getString("product_id"), ProductInfoEmpty));
                if(result.second) {
                    result.first->second.id = result.first->first;
                    result.first->second.price = product.getFloat("product_price", 100.0);
                    result.first->second.value = product.getObject("product_desc").asInt();
                    result.first->second.name = product.getString("product_name");
                    result.first->second.desc = product.getString("product_desc");
                }
            });

            callback(true);
        }
        else {
            callback(false);
        }
    });*/

}

std::vector<ol::ProductInfo> ol::Payment::getProductList(void)
{
    std::vector<inewsoft::ol::ProductInfo> productList;
    for(auto iter = this->productTable.begin(); iter != this->productTable.end(); ++iter)
    {
        productList.push_back(iter->second);
    }
    std::sort(productList.begin(), productList.end(), [](const ProductInfo& left, const ProductInfo& right)->bool{
        return left.value < right.value;
    });
    return std::move(productList);
}


