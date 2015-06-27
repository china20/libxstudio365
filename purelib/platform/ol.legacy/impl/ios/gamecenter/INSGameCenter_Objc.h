#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface CCGameCenter_Objc : NSObject

+ (CCGameCenter_Objc *)sharedGameCenter;
+ (void)purgeSharedGameCenter;

/// 判断GameCenter是否可用
- (BOOL) isGameCenterAvailable;

/// 用户登录GameCenter
- (void) authenticateLocalPlayer;

/// 注册用户变更检测
- (void) registerForAuthenticationNotification;

/// 用户变更处理
- (void) authenticationChanged;

/// 上传分数
- (void) reportScore: (int64_t) score forCategory: (NSString*) category;

/// 下载分数
- (void) retrieveScores: (int) lowerBound forUpperBound: (int) upperBound;

/// 获取好友列表
- (void) retrieveFriends;

/// 加载玩家数据
- (void) loadPlayerData: (NSArray*) identifiers;

/// 提交成就,
/// params:
///     identifier: 成就id
///     percent:    成就进度
- (void) reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent;

/// 下载成就
- (void) loadAchievements;

/// 根据Id获取成就信息
- (GKAchievement*) getAchievementForIdentifier: (NSString*) identifier;

/// 获取成就图片
- (NSArray*)retrieveAchievmentMetadata;

@end
