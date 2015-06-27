#include "../../../INSOLLocationManager.h"
#include "INSLocationDelegate.h"

NS_INS_OL_BEGIN

class LocationManager_iOS : public LocationManager, public LocationDelegate
{
public:
    bool init(void);
    virtual bool isLocationAvailable(void) const;
    virtual void startLocate(const LocationUpdateCallback& callback);
    virtual void stopLocate(void);
    virtual void onLocationUpdate(bool succeed, const CCPoint& location);
};

NS_INS_OL_END;


