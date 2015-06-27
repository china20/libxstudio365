#include "NxPayment_Win32.h"
#include "thelib/utils/nsconv.h"
#include "thelib/utils/crypto_wrapper.h"
#include "thelib/CCFloatingText.h"
#include "HttpAgent.h"



NxPayment_Win32::NxPayment_Win32(void) : bIsProductsLoaded(false),bIsPurchasePending(false),
    bIsPurchaseSent(false)
{
}

bool NxPayment_Win32::init(void)
{ // do nothing
    return true;
}

void  NxPayment_Win32::payForProduct(const char* productId, const TransactionCallback& callback)
{
    createTextTagForWarnning("Payment Not Support On Win32");

    if(callback)
        callback(true, "", "");
}
