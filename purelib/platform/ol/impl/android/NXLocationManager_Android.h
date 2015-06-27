#include "NxLocationManager.h"
#include "CCLocationDelegate.h"
#include "ProtocolLM.h"

class NxLocationManager_Android : public NxLocationManager, public cocos2d::plugin::LocationListener
{
public:

	virtual ~NxLocationManager_Android(void);

    bool init(void);
  
    virtual bool isLocationAvailable(void) const;
    virtual void startLocate(const LocationUpdateCallback& callback);
    virtual void stopLocate(void);
    virtual void onLocateResult(int code, double latitude, double longitude);
};

