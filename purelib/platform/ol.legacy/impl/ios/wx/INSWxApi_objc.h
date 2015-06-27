#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "WxApi.h"
#import "INSWxApiObject.h"

@interface INSWxApi_objc : UIResponder<UIApplicationDelegate, WXApiDelegate>
{
    enum WXScene scene;
    INSWxApiDelegate* delegate;
}

+ (UIImage *) imageFromCCImage:(cocos2d::CCImage *) ccImage;

//／ 默认分享到朋友圈
+(INSWxApi_objc*) sharedWxApi;
+(void) purgeSharedWxApi;

-(void) setDelegate:(INSWxApiDelegate*)apiDelegate;
- (void) changeScene:(NSInteger)theScene;
- (void) sendTextContent:(NSString*)content;
- (void) sendLinkContent:(NSString*)title andDesc:(NSString*)description andUrl:(NSString*)url andImage:(NSString*)image;
- (void) sendImageContent:(cocos2d::CCImage*)image andThumbImage:(cocos2d::CCImage*)thumbImage;
- (void) sendMusicContent;
- (void) sendVideoContent;
- (void) sendAppContent;
- (void) sendNonGifContent;
- (void) sendGifContent;
- (void) sendFileContent;

- (void) respTextContent;
- (void) respImageContent;
- (void) respLinkContent;
- (void) respMusicContent;
- (void) respVideoContent;
- (void) respAppContent;
- (void) respNonGifContent;
- (void) respGifContent;
- (void) respFileContent;

/* callback */
-(void) onReq:(BaseReq*)req;
-(void) onResp:(BaseResp*)resp;

/* wx notify entrys */
- (BOOL) handleOpenURL:(NSURL *)url;
- (BOOL) openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;



@end


