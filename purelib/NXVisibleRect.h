//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _NXVISIBLERECT_H_
#define _NXVISIBLERECT_H_

#include "cocos2d.h"
USING_NS_CC;
namespace purelib {
class VisibleRect
{
public:
    static void   refresh(void);

    static cocos2d::Rect getVisibleRect();
    static cocos2d::Size size();
    static cocos2d::Point left();
    static cocos2d::Point right();
    static cocos2d::Point top();
    static cocos2d::Point bottom();
    static cocos2d::Point center();
    static cocos2d::Point leftTop();
    static cocos2d::Point rightTop();
    static cocos2d::Point leftBottom();
    static cocos2d::Point rightBottom();

private:
    static void lazyInit();
    static cocos2d::Rect s_visibleRect;
};

};

#endif /* _INS_VISIBLERECT_H_*/
