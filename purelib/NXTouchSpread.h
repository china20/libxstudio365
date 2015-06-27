#ifndef _CCTOUCHABLESPRITE_H_
#define _CCTOUCHABLESPRITE_H_

#include <cocos2d.h>
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CCTouchSpread
{
public:
    CCTouchSpread(void);
    virtual ~CCTouchSpread(void);

	/// 增大响应范围，使目标控件能够响应精灵触摸时间
	void attach(Control* button);

    bool containsTouchLocation(CCTouch* touch);
    virtual bool onTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void onTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void onTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void onTouchCancelled(CCTouch* touch, CCEvent* event);

private:
	Control*              control;
};

#endif
