#include "INSOLPayment.h"
#include "thelib/INSCCSingleton.h"
#include <algorithm>
#include <cocos2d.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "INSPaymentImpl_iOS.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//#include "INSPayment_Win32.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
/// android default use alipay
#if defined(USE_GOOGLEPAY)
#include "INSPayment_GooglePay.h"
#else
#include "INSPayment_Alipay.h"
#endif

#endif

inewsoft::ol::Payment* inewsoft::ol::getPayment(void)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    //return CCSingleton<INSPaymentImpl_iOS>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    return NULL;// CCSingleton<ins::INSPayment_Win32>::instance();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#if defined(USE_GOOGLEPAY)
    return CCSingleton<INSPayment_GooglePay>::instance();
#else
    return CCSingleton<INSPayment_Alipay>::instance();
#endif
#else
    return nullptr;
#endif
}

inewsoft::ol::ProductInfo inewsoft::ol::ProductInfoEmpty = inewsoft::ol::ProductInfo();

void inewsoft::ol::Payment::loadProducts(inewsoft::ol::LoadProductCallback callback)
{
    /*NxHttpAgent::post(NxHttpAgent::makeJsonForGetProductList(), [this,callback](int ret, const cjsonw::object& params){

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

std::vector<inewsoft::ol::ProductInfo> inewsoft::ol::Payment::getProductList(void)
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


