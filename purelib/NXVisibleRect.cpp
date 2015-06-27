//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#include "NXVisibleRect.h"
#include "NXNodesLayout.h"

namespace purelib {

cocos2d::Rect VisibleRect::s_visibleRect;

void VisibleRect::refresh(void)
{
    auto director = Director::getInstance();
    s_visibleRect.origin = Director::getInstance()->getVisibleOrigin();
    s_visibleRect.size = Director::getInstance()->getVisibleSize();
}

void VisibleRect::lazyInit()
{
    if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
    {
        auto director = Director::getInstance();
        auto glview = director->getOpenGLView();

        if (glview->getResolutionPolicy() == kResolutionNoBorder) {
            s_visibleRect.origin = director->getVisibleOrigin();
            s_visibleRect.size = director->getVisibleSize();
        }
        else {
            s_visibleRect.origin = nodes_layout::getVisibleOrigin();
            s_visibleRect.size = nodes_layout::getVisibleSize();
        }
    }
}

cocos2d::Rect VisibleRect::getVisibleRect()
{
    lazyInit();
    return cocos2d::Rect(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

cocos2d::Size VisibleRect::size()
{
    lazyInit();
    return s_visibleRect.size;
}

Point VisibleRect::left()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::right()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::top()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::bottom()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

Point VisibleRect::center()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

Point VisibleRect::leftTop()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::rightTop()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

Point VisibleRect::leftBottom()
{
    lazyInit();
    return s_visibleRect.origin;
}

Point VisibleRect::rightBottom()
{
    lazyInit();
    return cocos2d::Point(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}

};


