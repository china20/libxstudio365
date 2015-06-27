//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _NXNODESUTILITY_H_
#define _NXNODESUTILITY_H_
#include <cocos2d.h>
#include <algorithm>
#include "utils/politedef.h"
#include "NXSharedPtr.h"
#include "deprecated/CCDeprecated.h"
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class nodes_utility {
    PLIB_STATICIZE_CLASS(nodes_utility);

    /// retrun a object with mark autorelease
    static ControlButton*   a_button(const char* normalImage = nullptr, const char* highlightedImage = nullptr, const char* selectedImage = nullptr, const char* disabledImage = nullptr);
    static ControlButton*   a_button(const char* title, const char* titleFontName, float titleFontSize, const Color3B& titleColor = Color3B::WHITE);

    static Label*           a_label(const char* string, const char* fontName, float fontSize, const Color3B& rgb = Color3B::WHITE);
   
    static Sprite*          a_lump(const Color4B& color = Color4B::WHITE, int height = 48, int width = 6);

    static void             changeNodeParent(Node* target, Node* newParent);

    static bool             checkVisibility(Node* theNode);

    
    static bool             containsTouchPoint(cocos2d::Node* target, cocos2d::Touch* touch);

    static bool             containsTouchPointAR(cocos2d::Node* target, cocos2d::Touch* touch);

    static bool             containsTouchPointPR(cocos2d::Node* target, cocos2d::Touch* touch);

    /*
    ** bool touched = nodes_utility::getNodeRectInWorld(pNode).containsPoint(touch->getLocation());
    */
    static Rect             getNodeRectInWorld(Node * pNode);

    static Rect             getNodeRect(Node * pNode);

    static Rect             getNodeRectAR(Node* pNode);

    static cocos2d::Rect    unreverseRect(const cocos2d::Rect& rect);

    static void             setScale2D(Node* pNode, const cocos2d::Vec2& scale){ pNode->setScaleX(scale.x); pNode->setScaleY(scale.y); }
    static cocos2d::Vec2    getScale2D(Node* pNode){ return cocos2d::Vec2(pNode->getScaleX(), pNode->getScaleY()); }

    static void             setScale3D(Node* pNode, const cocos2d::Vec3& scale){ pNode->setScaleX(scale.x); pNode->setScaleY(scale.y); pNode->setScaleZ(scale.z); }
    static cocos2d::Vec3    getScale3D(Node* pNode){ return cocos2d::Vec3(pNode->getScaleX(), pNode->getScaleY(), pNode->getScaleZ()); }

    /*
    ** snapshot specific Node at this frame
    */
    static cocos2d::Image*  snapshot(Node*, float scale = .5f);

    /*
    ** snapshot screen  at this frame
    */
    static cocos2d::Image*  snapshot(float scale = .5f);


    static cocos2d::Image*  snapshot_v2(float scale = .5f);
    

    /*
    ** snapshot specific Node at next frame
    */
    static void  snapshot(const std::string& fileName, Node*, float scale = .5f, const std::function<void(cocos2d::Image* image)>& callback = nullptr);

    /*
    ** snapshot screen at next frame
    */
    static void  snapshot(const std::string& fileName, float scale = .5f, const std::function<void(cocos2d::Image* image)>& callback = nullptr);

    template<typename _Impl> inline
    static cocos2d::RefPtr<_Impl> shared(_Impl* target)
    {
        return cocos2d::RefPtr<_Impl>(target);
    }

}; /* STATIC CLASS nodes_layout */

typedef nodes_utility nut;

#endif

