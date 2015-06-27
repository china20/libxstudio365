#ifndef _INS_LOCATION_DELEGATE_H_
#define _INS_LOCATION_DELEGATE_H_
#include <thelib/INSDefs.h>

NS_INS_OL_BEGIN

class LocationDelegate
{
public:
    virtual void onLocationUpdate(bool succeed, const CCPoint& location) = 0;
};

NS_INS_OL_END;

#endif

