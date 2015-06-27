//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _NXCIRCLE_H_
#define _NXCIRCLE_H_
 
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

namespace purelib {
 
class Circle
{
public:
	Circle(void);
    Circle(const cocos2d::Point& center, float radius);
    ~Circle(void);
 
    bool intersectsRect(const cocos2d::Rect& rect) const;
    bool intersectsRectNearest(const cocos2d::Rect& rect) const;
    bool intersectsRectAny(const cocos2d::Rect& rect) const;
    bool intersectsCircle(const Circle& circle) const;
 
    bool containsPoint(const Point& p) const; 
private:
    CC_SYNTHESIZE(cocos2d::Point,m_center,Center);
    CC_SYNTHESIZE(float,m_radius,Radius);
};
 
#define CCCircMake(ccPoint, radius) Circle::Circle(ccPoint, (float)(radius))

};


#endif
