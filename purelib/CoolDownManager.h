#ifndef _COOLDOWNMANAGER_H_
#define _COOLDOWNMANAGER_H_
#include <cocos2d.h>
#include "ui/UIButton.h"

using namespace cocos2d::ui;
class CoolDownManager : public cocos2d::Ref
{
public:
	CoolDownManager();
	~CoolDownManager();

	void coolDown(Button* target, float cd, const  std::function<void()>& callback = nullptr);

	void pause();
	void resume();	

	// don't call this function manually.
	void update(float dt);

	void cleanup(void);

	bool hasTheBtn( Button* btn);
private:

	cocos2d::Scheduler* scheduler;

	cocos2d::Vector<Button*> waitings;

	std::map<Button*, std::function<void()> > callFun;
};

#define cooldown_manager cocos2d::Singleton<CoolDownManager>::instance()

#endif

