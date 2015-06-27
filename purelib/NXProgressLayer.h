/****************************************************************************
Copyright (c) 2014 purelib corp.
****************************************************************************/

#ifndef _INS_PROGRESSLAYER_H_
#define _INS_PROGRESSLAYER_H_

#include "cocos2d.h"

USING_NS_CC;

enum {
	kCCProgressLayerTag = 31237
};

class ActivityIndicator;

class ProgressLayer : public cocos2d::CCLayerColor {
public:
    bool init();
    CREATE_FUNC(ProgressLayer);
    ~ProgressLayer();
    void show();
    void hide();
    bool isShowing() {return showing;}

	void attach(cocos2d::Node* node);
	void detach(cocos2d::Node* node);

	virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
private:
    bool showing;
    ActivityIndicator * activityIndicator;
};

#endif