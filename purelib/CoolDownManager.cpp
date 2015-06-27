#include "CoolDownManager.h"
#include "purelib/utils/super_cast.h"

CoolDownManager::CoolDownManager() : scheduler(cocos2d::Director::getInstance()->getScheduler())
{
	scheduler->scheduleUpdate(this, (std::numeric_limits<int>::min)(), true);
}

CoolDownManager::~CoolDownManager()
{
	scheduler->unscheduleUpdate(this);
}

void CoolDownManager::coolDown(Button* target, float duration, const std::function<void()>& callback)
{
	target->setEnabled(false);
	target->setUserObject(cocos2d::Float::create(duration));
	target->setUserData(purelib::super_cast::uniform_cast<void*>(duration));

	waitings.pushBack(target);
	if (waitings.size() == 1)
		this->resume();

	if (callback)
	{
		callFun.insert(std::make_pair(target, callback));
	}
}

void CoolDownManager::update(float dt)
{
	for (auto iter = waitings.begin(); iter != waitings.end();)
	{
		auto item = *iter;
		auto maxVal = dynamic_cast<cocos2d::Float*>(item->getUserObject())->getValue();

		auto nowVal = purelib::super_cast::uniform_cast<float>(item->getUserData());
		if ((nowVal -= dt) <= .0f)
		{
			item->setUserObject(nullptr);
			item->setUserData(nullptr);
			item->setEnabled(true);
			auto call = callFun.find(item);
			if (call != callFun.end())
			{
				call->second();
				callFun.erase(call);
			}
			iter = waitings.erase(iter);
			continue;
		}

		item->getVirtualRenderer()->getGLProgramState()->setUniformFloat("u_percent", (nowVal / maxVal));
		item->setUserData(purelib::super_cast::uniform_cast<void*>(nowVal));
		++iter;
	}

	if (waitings.empty())
	{
		this->pause();
	}
}

void CoolDownManager::resume()
{
	scheduler->resumeTarget(this);
}

void CoolDownManager::pause()
{
	scheduler->pauseTarget(this);
}

void CoolDownManager::cleanup(void)
{
	this->pause();
	callFun.clear();
	this->waitings.clear();
}

bool CoolDownManager::hasTheBtn( Button* btn)
{
	return !( waitings.find(btn) == waitings.end());
}

