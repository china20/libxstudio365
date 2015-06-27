#include "INSLocationManager_iOS.h"
#import "INSLocationManager_objc.h"

USING_NS_INS_OL;

bool LocationManager_iOS::init(void)
{
    [[INSLocationManager_objc sharedManager] setDelegate:this];
    return true;
}

bool LocationManager_iOS::isLocationAvailable(void) const
{
    return [[INSLocationManager_objc sharedManager] isLocationAvailable];
}

void LocationManager_iOS::startLocate(const LocationUpdateCallback& callback)
{
    this->callback = callback;
    
    [[INSLocationManager_objc sharedManager] startLocate];
}

void LocationManager_iOS::stopLocate(void)
{
    [[INSLocationManager_objc sharedManager] stopLocate];
}

void LocationManager_iOS::onLocationUpdate(bool succeed, const CCPoint& location)
{
    if(this->callback) {
        this->callback(succeed, location);
        this->callback = nullptr;
    }
}
