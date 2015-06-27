#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <MapKit/MapKit.h>

#include "INSLocationDelegate.h"

@interface INSLocationManager_objc : NSObject<CLLocationManagerDelegate>
{
    inewsoft::ol::LocationDelegate* delegate;
}

+(INSLocationManager_objc*) sharedManager;
+(void) purgeSharedManager;

-(void) setDelegate:(inewsoft::ol::LocationDelegate*)theDelegate;

-(bool) isLocationAvailable;

-(void) startLocate;

-(void) stopLocate;

@end


