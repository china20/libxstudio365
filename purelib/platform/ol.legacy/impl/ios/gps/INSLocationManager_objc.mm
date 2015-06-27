
#import "INSLocationManager_objc.h"


@implementation INSLocationManager_objc

static INSLocationManager_objc *s_sharedManager = nil;

static CLLocationManager* s_locationManager = nil;

+(INSLocationManager_objc*) sharedManager
{
    if (!s_sharedManager)
    {
        s_sharedManager = [[INSLocationManager_objc alloc] init];
        [s_sharedManager setDelegate:nil];
    }
    return s_sharedManager;
}

+(void) purgeSharedManager
{
    if (s_sharedManager)
    {
        [s_sharedManager release];
    }
}

-(void) setDelegate:(inewsoft::ol::LocationDelegate*)theDelegate
{
    delegate = theDelegate;
}

-(bool) isLocationAvailable
{
    return [CLLocationManager locationServicesEnabled];
}

-(void) startLocate
{
    if(![self isLocationAvailable] || s_locationManager != nil) {
        if(delegate)
            delegate->onLocationUpdate(false, CCPointZero);
        return;
    }
    
    s_locationManager = [[CLLocationManager alloc] init];
    
    [s_locationManager retain];
    
    // 设置定位精度，十米，百米，最好 kCLLocationAccuracyBest
    [s_locationManager setDesiredAccuracy:kCLLocationAccuracyNearestTenMeters];
    s_locationManager.delegate = self;
    
    // 开始时时定位
    [s_locationManager startUpdatingLocation];
}

-(void) stopLocate
{
    [s_locationManager stopUpdatingLocation];
    [s_locationManager release];
    s_locationManager = nil;
}


-(void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error {
    if(delegate)
        delegate->onLocationUpdate(false, CCPointZero);
}

// 6.0 以上调用这个函数
-(void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations {
    
    NSLog(@"%d", [locations count]);
    
    if([locations count] <= 0) {
        if(delegate)
            delegate->onLocationUpdate(false, CCPointZero);
        [manager stopUpdatingLocation];
        return;
    }
    
    CLLocation *newLocation = locations[0];
    CLLocationCoordinate2D oldCoordinate = newLocation.coordinate;
    // NSLog(@"旧的经度：%f,旧的纬度：%f",oldCoordinate.longitude,oldCoordinate.latitude);
    
    if(delegate)
        delegate->onLocationUpdate(true, CCPointMake(newLocation.coordinate.latitude, newLocation.coordinate.longitude));
    
    // [manager stopUpdatingLocation];
    
    //    CLLocation *newLocation = locations[1];
    //    CLLocationCoordinate2D newCoordinate = newLocation.coordinate;
    //    NSLog(@"经度：%f,纬度：%f",newCoordinate.longitude,newCoordinate.latitude);
    
    // 计算两个坐标距离
    //    float distance = [newLocation distanceFromLocation:oldLocation];
    //    NSLog(@"%f",distance);
    
    // [manager stopUpdatingLocation];
    
    //------------------位置反编码---5.0之后使用-----------------
    /*CLGeocoder *geocoder = [[CLGeocoder alloc] init];
     [geocoder reverseGeocodeLocation:newLocation
     completionHandler:^(NSArray *placemarks, NSError *error){
     
     for (CLPlacemark *place in placemarks) {
     // UILabel *label = (UILabel *)[self.window viewWithTag:101];
     // label.text = place.name;
     //  NSLog(@"name,%@",place.name);                       // 位置名
     //                           NSLog(@"thoroughfare,%@",place.thoroughfare);       // 街道
     //                           NSLog(@"subThoroughfare,%@",place.subThoroughfare); // 子街道
     //                           NSLog(@"locality,%@",place.locality);               // 市
     //                           NSLog(@"subLocality,%@",place.subLocality);         // 区
     //                           NSLog(@"country,%@",place.country);                 // 国家
     }
     
     }];*/
    
}

// 6.0 调用此函数
-(void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation {
    // NSLog(@"%@", @"ok");
    // [manager stopUpdatingLocation];
    
    if(delegate)
        delegate->onLocationUpdate(true, CCPointMake(newLocation.coordinate.latitude, newLocation.coordinate.longitude));
}



@end

