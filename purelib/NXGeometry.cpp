//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#include "NXGeometry.h"

#include "cocos2d.h"

#define QUADRATIC(n) ( (n) * (n) )

USING_NS_CC;

purelib::Circle::Circle(void) :
m_center(Point(0, 0)), m_radius(0)
{
}

purelib::Circle::Circle(const cocos2d::Point& center, float radius) :
m_center(center), m_radius(radius){
}

purelib::Circle::~Circle(void)
{
}

bool purelib::Circle::intersectsRect(const cocos2d::Rect& rect) const
{
    /*
    **
    ** algorithm come from: http://blog.csdn.net/teng_ontheway/article/details/38706837
    **
    */
    auto rcMidX = rect.getMidX();
    auto rcMidY = rect.getMidY();

    //1
    auto m_centerDistanceX = abs(m_center.x - rcMidX);
    auto m_centerDistanceY = abs(m_center.y - rcMidY);

    //2
    if (m_centerDistanceX > (rect.size.width / 2 + m_radius)) { return false; }
    if (m_centerDistanceY > (rect.size.height / 2 + m_radius)) { return false; }

    //3
    if (m_centerDistanceX <= (rect.size.width / 2)) { return true; }
    if (m_centerDistanceY <= (rect.size.height / 2)) { return true; }

    //4
    return ((QUADRATIC(m_centerDistanceX - rect.size.width / 2) + QUADRATIC(m_centerDistanceY - rect.size.height / 2)) <= (QUADRATIC(m_radius)));
    //float arcR = (float)m_radius;//圆形半径
    //float arcOx = (float)m_center.x;//圆心X坐标
    //float arcOy = (float)m_center.y;//圆心Y坐标
    //float rectX = (float)rect.getMinX();//长方形左上角X坐标
    //float rectY = (float)rect.getMaxY();//长方形右上角Y坐标
    //float rectW = rect.size.width;//长方形宽
    //float rectH = rect.size.height;//长方形高

    //if(((rectX-arcOx) * (rectX-arcOx) + (rectY-arcOy) * (rectY-arcOy)) <= arcR * arcR)
    //    return true;
    //if(((rectX+rectW-arcOx) * (rectX+rectW-arcOx) + (rectY-arcOy) * (rectY-arcOy)) <= arcR * arcR)
    //    return true;
    //if(((rectX-arcOx) * (rectX-arcOx) + (rectY+rectH-arcOy) * (rectY+rectH-arcOy)) <= arcR * arcR)
    //    return true;
    //if(((rectX+rectW-arcOx) * (rectX+rectW-arcOx) + (rectY+rectH-arcOy) * (rectY+rectH-arcOy)) <= arcR * arcR)
    //    return true;
    ////分别判断矩形4个顶点与圆心的距离是否<=圆半径；如果<=，说明碰撞成功


    //float minDisX = 0;
    //if(arcOy >= rectY && arcOy <= rectY + rectH){
    //    if(arcOx < rectX)
    //        minDisX = rectX - arcOx;
    //    else if(arcOx > rectX + rectW)
    //        minDisX = arcOx - rectX - rectW;
    //    else 
    //        return true;
    //    if(minDisX <= arcR)
    //        return true;
    //}//判断当圆心的Y坐标进入矩形内时X的位置，如果X在(rectX-arcR)到(rectX+rectW+arcR)这个范围内，则碰撞成功

    //float minDisY = 0;
    //if(arcOx >= rectX && arcOx <= rectX + rectW){
    //    if(arcOy < rectY)
    //        minDisY = rectY - arcOy;
    //    else if(arcOy > rectY + rectH)
    //        minDisY = arcOy - rectY - rectH;
    //    else
    //        return true;
    //    if(minDisY <= arcR)
    //        return true;
    //}//判断当圆心的X坐标进入矩形内时Y的位置，如果X在(rectY-arcR)到(rectY+rectH+arcR)这个范围内，则碰撞成功
    //return false;
}

bool purelib::Circle::intersectsRectNearest(const cocos2d::Rect& rect) const//是否与长方形区域相交
{
    float cx = 0;
    float cy = 0;

    // Find the point on the collision box closest to the center of the m_center
    if (m_center.x < rect.origin.x)
        cx = rect.origin.x;
    else if (m_center.x >(rect.origin.x + rect.size.width))
        cx = rect.origin.x + rect.size.width;
    else
        cx = m_center.x;

    if (m_center.y < rect.origin.y)
        cy = rect.origin.y;
    else if (m_center.y >(rect.origin.y + rect.size.height))
        cy = rect.origin.y + rect.size.height;
    else
        cy = m_center.y;

    return (QUADRATIC(m_center.x - cx) + QUADRATIC(m_center.y - cy)) <= QUADRATIC(m_radius);

    // return m_center.distance(Vec2(cx, cy)) <= m_radius;
}

bool purelib::Circle::intersectsRectAny(const cocos2d::Rect& rect) const
{ // http://www.migapro.com/circle-and-rotated-rectangle-collision-detection/
    auto rcAngle = rect.origin.getAngle(); // radians
    auto rcCenterX = rect.getMidX();
    auto rcCenterY = rect.getMidY();

    // Rotate m_center's center point back
    double unrotatedm_centerX = std::cos(rcAngle) * (m_center.x - rcCenterX) -
        std::sin(rcAngle) * (m_center.y - rcCenterY) + rcCenterX;
    double unrotatedm_centerY = std::sin(rcAngle) * (m_center.x - rcCenterX) +
        std::cos(rcAngle) * (m_center.y - rcCenterY) + rcCenterY;

    // Closest point in the rectangle to the center of m_center rotated backwards(unrotated)
    double closestX, closestY;

    // Find the unrotated closest x point from center of unrotated m_center
    if (unrotatedm_centerX  < rect.origin.x)
        closestX = rect.origin.x;
    else if (unrotatedm_centerX  > rect.origin.x + rect.size.width)
        closestX = rect.origin.x + rect.size.width;
    else
        closestX = unrotatedm_centerX;

    // Find the unrotated closest y point from center of unrotated m_center
    if (unrotatedm_centerY < rect.origin.y)
        closestY = rect.origin.y;
    else if (unrotatedm_centerY > rect.origin.y + rect.size.height)
        closestY = rect.origin.y + rect.size.height;
    else
        closestY = unrotatedm_centerY;

    // Determine collision
    return (QUADRATIC(unrotatedm_centerX - closestX) + QUADRATIC(unrotatedm_centerY - closestY)) <= QUADRATIC(m_radius);
}

bool purelib::Circle::intersectsCircle(const Circle& circle) const
{
    return !(sqrt(QUADRATIC(m_center.x - circle.m_center.x) + QUADRATIC(m_center.y - circle.m_center.y)) > m_radius + circle.m_radius);
}

bool purelib::Circle::containsPoint(const Point& p) const
{
    return !(sqrt(QUADRATIC(p.x - m_center.x) + QUADRATIC(p.y - m_center.y)) > m_radius);
}



