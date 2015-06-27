
#import "NXGameCenter_objc.h"

@implementation NXGameCenter_objc

static NXGameCenter_objc *s_sharedGameCenter;

#pragma mark -
#pragma init

+ (NXGameCenter_objc *)sharedGameCenter
{
    if (!s_sharedGameCenter)
    {
        s_sharedGameCenter = [[NXGameCenter_objc alloc] init];
    }
    return s_sharedGameCenter;
}

+ (void)purgeSharedGameCenter
{
    if (s_sharedGameCenter)
    {
        [s_sharedGameCenter release];
    }
}

#pragma mark -
#pragma mark static methods

- (BOOL)isGameCenterAvailable
{
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}

- (void) authenticateLocalPlayer
{
    [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
        if (error == nil) {
            //成功处理
            NSLog(@"成功");
            NSLog(@"1--alias--.%@",[GKLocalPlayer localPlayer].alias);
            NSLog(@"2--authenticated--.%d",[GKLocalPlayer localPlayer].authenticated);
            NSLog(@"3--isFriend--.%d",[GKLocalPlayer localPlayer].isFriend);
            NSLog(@"4--playerID--.%@",[GKLocalPlayer localPlayer].playerID);
            NSLog(@"5--underage--.%d",[GKLocalPlayer localPlayer].underage);
        }else {
            //错误处理
            NSLog(@"失败  %@",error);
        }
    }];
}

- (void) registerForAuthenticationNotification
{
    NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self
           selector:@selector(authenticationChanged)
               name:GKPlayerAuthenticationDidChangeNotificationName
             object:nil];
    
}

- (void) authenticationChanged
{
	
    if ([GKLocalPlayer localPlayer].isAuthenticated)
    {
        ;// Insert code here to handle a successful authentication.
    }
    else
    {
        ;// Insert code here to clean up any outstanding Game Center-related classes.
    }
    
}

- (void) reportScore: (int64_t) score forCategory: (NSString*) category
{
    GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];
    scoreReporter.value = score;
    
    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
        if (error != nil)
        {
            // handle the reporting error
            NSLog(@"上传分数出错.");
            //If your application receives a network error, you should not discard the score.
            //Instead, store the score object and attempt to report the player’s process at
            //a later time.
        }else {
            NSLog(@"上传分数成功");
        }
        
    }];
}

//GKScore objects provide the data your application needs to create a custom view.
//Your application can use the score object’s playerID to load the player’s alias.
//The value property holds the actual value you reported to Game Center. the formattedValue
//property provides a string with the score value formatted according to the parameters
//you provided in iTunes Connect.
- (void) retrieveScores: (int) lowerBound forUpperBound: (int) upperBound
{
    GKLeaderboard *leaderboardRequest = [[GKLeaderboard alloc] init];
    if (leaderboardRequest != nil)
    {
        leaderboardRequest.playerScope = GKLeaderboardPlayerScopeGlobal;
        leaderboardRequest.timeScope = GKLeaderboardTimeScopeAllTime;
        leaderboardRequest.range = NSMakeRange(lowerBound,upperBound);
        leaderboardRequest.category = @"TS_LB";
        [leaderboardRequest loadScoresWithCompletionHandler: ^(NSArray *scores, NSError *error) {
            if (error != nil){
                // handle the error.
                NSLog(@"下载失败");
            }
            if (scores != nil){
                // process the score information.
                NSLog(@"下载成功....");
                NSArray *tempScore = [NSArray arrayWithArray:leaderboardRequest.scores];
                for (GKScore *obj in tempScore) {
                    NSLog(@"    playerID            : %@",obj.playerID);
                    NSLog(@"    category            : %@",obj.category);
                    NSLog(@"    date                : %@",obj.date);
                    NSLog(@"    formattedValue    : %@",obj.formattedValue);
                    NSLog(@"    value                : %d",obj.value);
                    NSLog(@"    rank                : %d",obj.rank);
                    NSLog(@"**************************************");
                }
            }
        }];
    }
}


- (void) retrieveFriends
{
    GKLocalPlayer *lp = [GKLocalPlayer localPlayer];
    if (lp.authenticated)
    {
        [lp loadFriendsWithCompletionHandler:^(NSArray *friends, NSError *error) {
            if (error == nil)
            {
                [self loadPlayerData:friends];
            }
            else
            {
                ;// report an error to the user.
            }
        }];
        
    }
}


/*
 02.   Whether you received player identifiers by loading the identifiers for the local player’s
 03.  friends, or from another Game Center class, you must retrieve the details about that player
 04.  from Game Center.
 05.   */
- (void) loadPlayerData: (NSArray *) identifiers
{
    [GKPlayer loadPlayersForIdentifiers:identifiers withCompletionHandler:^(NSArray *players, NSError *error) {
        if (error != nil)
        {
            // Handle the error.
        }
        if (players != nil)
        {
            NSLog(@"得到好友的alias成功");
            GKPlayer *friend1 = [players objectAtIndex:0];
            NSLog(@"friedns---alias---%@",friend1.alias);
            NSLog(@"friedns---isFriend---%d",friend1.isFriend);
            NSLog(@"friedns---playerID---%@",friend1.playerID);
        }
    }];
}

- (void) reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent
{
    GKAchievement *achievement = [[[GKAchievement alloc] initWithIdentifier: identifier] autorelease];
    if (achievement)
    {
        achievement.percentComplete = percent;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error)
         {
             if (error != nil)
             {
                 //The proper way for your application to handle network errors is retain
                 //the achievement object (possibly adding it to an array). Then, periodically
                 //attempt to report the progress until it is successfully reported.
                 //The GKAchievement class supports the NSCoding protocol to allow your
                 //application to archive an achie
                 NSLog(@"报告成就进度失败 ,错误信息为: \n %@",error);
             }else {
                 //对用户提示,已经完成XX%进度
                 NSLog(@"报告成就进度---->成功!");
                 NSLog(@"    completed:%d",achievement.completed);
                 NSLog(@"    hidden:%d",achievement.hidden);
                 NSLog(@"    lastReportedDate:%@",achievement.lastReportedDate);
                 NSLog(@"    percentComplete:%f",achievement.percentComplete);
                 NSLog(@"    identifier:%@",achievement.identifier);
             }
         }];
    }
}


- (void) loadAchievements
{
    NSMutableDictionary *achievementDictionary = [[NSMutableDictionary alloc] init];
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements,NSError *error)
     {
         if (error == nil) {
             NSArray *tempArray = [NSArray arrayWithArray:achievements];
             for (GKAchievement *tempAchievement in tempArray) {
                 [achievementDictionary setObject:tempAchievement forKey:tempAchievement.identifier];
                 NSLog(@"    completed:%d",tempAchievement.completed);
                 NSLog(@"    hidden:%d",tempAchievement.hidden);
                 NSLog(@"    lastReportedDate:%@",tempAchievement.lastReportedDate);
                 NSLog(@"    percentComplete:%f",tempAchievement.percentComplete);
                 NSLog(@"    identifier:%@",tempAchievement.identifier);
             }
         }
     }];
}


- (GKAchievement*) getAchievementForIdentifier: (NSString*) identifier
{
    NSMutableDictionary *achievementDictionary = [[NSMutableDictionary alloc] init];
    GKAchievement *achievement = [achievementDictionary objectForKey:identifier];
    if (achievement == nil)
    {
        achievement = [[[GKAchievement alloc] initWithIdentifier:identifier] autorelease];
        [achievementDictionary setObject:achievement forKey:achievement.identifier];
    }
    return [[achievement retain] autorelease];
}


- (NSArray*)retrieveAchievmentMetadata
{
    //读取成就的描述
    [GKAchievementDescription loadAchievementDescriptionsWithCompletionHandler:
     ^(NSArray *descriptions, NSError *error) {
         if (error != nil)
         {
             // process the errors
             NSLog(@"读取成就说明出错");
         }
         if (descriptions != nil)
         {
             // use the achievement descriptions.
             for (GKAchievementDescription *achDescription in descriptions) {
                 NSLog(@"1..identifier..%@",achDescription.identifier);
                 NSLog(@"2..achievedDescription..%@",achDescription.achievedDescription);
                 NSLog(@"3..title..%@",achDescription.title);
                 NSLog(@"4..unachievedDescription..%@",achDescription.unachievedDescription);
                 NSLog(@"5............%@",achDescription.image);
                 
                 //获取成就图片,如果成就未解锁,返回一个大文号
                 /*
                  23.                  [achDescription loadImageWithCompletionHandler:^(UIImage *image, NSError *error) {
                  24.                      if (error == nil)
                  25.                      {
                  26.                          // use the loaded image. The image property is also populated with the same image.
                  27.                          NSLog(@"成功取得成就的图片");
                  28.                           UIImage *aImage = image;
                  29.                         UIImageView *aView = [[UIImageView alloc] initWithImage:aImage];
                  30.                          aView.frame = CGRectMake(50, 50, 200, 200);
                  31.                          aView.backgroundColor = [UIColor clearColor];
                  32.                          [[[CCDirector sharedDirector] openGLView] addSubview:aView];
                  33.                       }else {
                  34.                         NSLog(@"获得成就图片失败");
                  35.                      }
                  36.                 }];
                  37.                    */
             }
         }
     }];
    return nil;
}


@end