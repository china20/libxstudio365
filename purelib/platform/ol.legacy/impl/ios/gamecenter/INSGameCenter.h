#ifndef _CCGAMECENTER_H_
#define _CCGAMECENTER_H_

class CCGameCenter
{
public:
    static CCGameCenter* sharedGameCenter();
    static void purgeSharedGameCenter();
    
    bool isGameCenterAvailable();
    
    void authenticateLocalPlayer();
    
    //void registerForAuthenticationNotification();
    
    //void reportScore: (int64_t) score forCategory: (NSString*) category;
    
    //void retrieveFriends();
    
    //void loadPlayerData: (NSArray*) identifiers;
    
    //void reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent;
    
    //void loadAchievements();
    
    //- (GKAchievement*) getAchievementForIdentifier: (NSString*) identifier;
    
    //- (NSArray*)retrieveAchievmentMetadata;
    
    //void authenticationChanged();
};

#endif
