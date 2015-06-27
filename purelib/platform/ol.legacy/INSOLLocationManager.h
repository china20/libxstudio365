#ifndef _INS_OL_LOCATIONMANAGER_H_
#define _INS_OL_LOCATIONMANAGER_H_
#include "cocos2d.h"
#include <functional>
#include <thelib/INSDefs.h>

NS_INS_OL_BEGIN

typedef std::function<void(bool succeed, const CCPoint&)> LocationUpdateCallback;

class LocationManager : public cocos2d::Ref
{
public:
    virtual ~LocationManager(void) {};

    virtual bool init(void) {return true;}
    virtual bool isLocationAvailable(void) const { return false; };
    virtual void startLocate(const LocationUpdateCallback& callback){if(callback) callback(false, CCPointZero);};
    virtual void stopLocate(void) {};
    
protected:
    LocationUpdateCallback callback;
};

NS_INS_OL_END;

extern inewsoft::ol::LocationManager* getLocationManager(void);

#define gps getLocationManager()

#endif

