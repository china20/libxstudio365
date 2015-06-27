//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#include "NXSharedPtr.h"
#include "NXNodesLayout.h"
#include "NXNodesUtility.h"
#include "NXColorUtility.h"
#include <cmath>

#if 0
/**
 * define a create function for a specific type, such as CCLayer
 * @__TYPE__ class type to add create(), such as CCLayer
 */
#define IMPL_NEW_NODE(__TYPE__) \
do { \
    __TYPE__* pRet(new(std::nothrow) __TYPE__()); \
    if( pRet ) \
    { \
        pRet->init(); \
        return pRet; \
    } \
    return pRet; \
} while(false) 

ControlButton*         nodes_utility::a_button(const char* normalImage, const char* highlightedImage, const char* selectedImage, const char* disabledImage)
{
    return new_button(normalImage, highlightedImage, selectedImage, disabledImage));
}

ControlButton*         nodes_utility::a_button(const char* title, const char* titleFontName, float titleFontSize, const Color3B& titleColor)
{
    return new_button(title, titleFontName, titleFontSize, titleColor));
}

Node*            nodes_utility::new_node(void)
{
    IMPL_NEW_NODE(Node);
}
cocos2d::Scene*           nodes_utility::new_scene(void)
{
    IMPL_NEW_NODE(cocos2d::Scene);
}
CCLayer*           nodes_utility::new_layer(void)
{
    IMPL_NEW_NODE(CCLayer);
}

CCSpriteFrame*     nodes_utility::new_sprite_frame(const char* pszFilename, const cocos2d::Rect& rect)
{
    auto pRet = new(std::nothrow) CCSpriteFrame();
    if(pRet) {
        pRet->initWithTextureFilename(pszFilename, rect);
        return pRet;
    }
    return pRet;
}
CCSpriteFrame*     nodes_utility::new_sprite_frame(Texture2D* texture, const cocos2d::Rect& rect)
{
    auto pRet = new(std::nothrow) CCSpriteFrame();
    if(pRet) {
        pRet->initWithTexture(texture, rect);
        return pRet;
    }
    return pRet;
}

Sprite*          nodes_utility::new_sprite(const char* pszFilename)
{
    auto pRet = new(std::nothrow) Sprite();
    if(pRet) {
        pRet->initWithFile(pszFilename);
        return pRet;
    }
    return pRet;
}
Sprite*  nodes_utility::new_sprite(const char* pszFilename, const cocos2d::Rect& rect)
{
    auto pRet = new(std::nothrow) Sprite();
    if(pRet) {
        pRet->initWithFile(pszFilename, rect);
        return pRet;
    }
    return pRet;
}
Sprite*          nodes_utility::new_sprite(Texture2D* texture)
{
    auto pRet = new(std::nothrow) Sprite();
    if(pRet) {
        pRet->initWithTexture(texture);
        return pRet;
    }
    return pRet;
}
Sprite*          nodes_utility::new_sprite(Texture2D* texture, const cocos2d::Rect& rect)
{
    auto pRet = new(std::nothrow) Sprite();
    if(pRet) {
        pRet->initWithTexture(texture, rect);
        return pRet;
    }
    return pRet;
}
Sprite*          nodes_utility::new_sprite(CCSpriteFrame* spriteFrame)
{
    auto pRet = new(std::nothrow) Sprite();
    if(pRet) {
        pRet->initWithSpriteFrame(spriteFrame);
        return pRet;
    }
    return pRet;
}
Sprite* nodes_utility::new_spritef(const char* pszSpriteFrameName)
{
    auto pRet = new(std::nothrow) Sprite();
    if(pRet) {
        pRet->initWithSpriteFrameName(pszSpriteFrameName);
        return pRet;
    }
    return pRet;
}
Scale9Sprite*    nodes_utility::new_scale9sprite(const char* pszFilename)
{
    auto pRet = new(std::nothrow) Scale9Sprite();
    if(pRet) {
        pRet->initWithFile(pszFilename);
        return pRet;
    }
    return pRet;
}
CCLayerColor*      nodes_utility::new_layercolor(const Color4B& rgba)
{
    auto pRet = new(std::nothrow) CCLayerColor();
    if(pRet) {
        pRet->initWithColor(rgba);
        return pRet;
    }
    return pRet;
}
Label*        nodes_utility::new_label(const char* string, const char* fontName, float fontSize, const Color3B& rgb)
{
 auto pRet = new(std::nothrow) Label();
    if(pRet) {
        pRet->initWithString(string, fontName, fontSize);
        pRet->setColor(rgb);
        return pRet;
    }
    return pRet;
}
#endif


ControlButton*   nodes_utility::a_button(const char* normalImage, const char* highlightedImage, const char* selectedImage, const char* disabledImage)
{
    ControlButton* button = ControlButton::create();
    if (button == nullptr)
        return nullptr;

    SharedPtr<Scale9Sprite> background;
    if(normalImage != nullptr)
    {
        background.reset(new Scale9Sprite());
        background->initWithFile(normalImage);
        cocos2d::Size preferedSize = background->getContentSize();
        button->setBackgroundSpriteForState(background, Control::State::NORMAL);
        button->setAdjustBackgroundImage(true);
        button->setPreferredSize(preferedSize);
    }
    if(highlightedImage != nullptr)
    {
        background.reset(new Scale9Sprite());
        background->initWithFile(highlightedImage);
        button->setBackgroundSpriteForState(background, Control::State::HIGH_LIGHTED);
    }
    if(selectedImage != nullptr)
    {
        background.reset(new Scale9Sprite());
        background->initWithFile(selectedImage);
        button->setBackgroundSpriteForState(background, Control::State::SELECTED);
    }
    if(disabledImage != nullptr)
    {
        background.reset(new Scale9Sprite());
        background->initWithFile(disabledImage);
        button->setBackgroundSpriteForState(background, Control::State::DISABLED);
    }
    return button;
}
ControlButton*   nodes_utility::a_button(const char* title, const char* titleFontName, float titleFontSize, const Color3B& titleColor)
{
    ControlButton* button = a_button();
    // auto ccTitle = CCString::create(title);
    button->setTitleForState(title, Control::State::NORMAL);
    button->setTitleTTFForState(titleFontName, Control::State::NORMAL);
    button->setTitleTTFSizeForState(titleFontSize, Control::State::NORMAL);
    button->setTitleColorForState(titleColor, Control::State::NORMAL);
    return button;
}

Label* nodes_utility::a_label(const char* string, const char* fontName, float fontSize, const Color3B& rgb)
{
    auto lbl = Label::create(string, fontName, fontSize);
    lbl->setColor(rgb);
    return lbl;
}

Sprite* nodes_utility::a_lump(const Color4B& color, int height, int width)
{
    unsigned int* pixels((unsigned int*)malloc(height * width * sizeof(unsigned int)) );

    // Fill Pixels
    uint32_t* ptr = pixels;
    for (int i = 0; i < height * width; ++i) 
    {
        ptr[i] = color_utility::c4b2dw(color);// 0xffffffff;
    }

    // create cursor by pixels
    Texture2D* texture = new Texture2D();

    texture->initWithData(pixels, height * width * sizeof(unsigned int), Texture2D::PixelFormat::RGBA8888, width, height, Size(width, height));

    auto cursor = Sprite::createWithTexture(texture);

    /*CCAction* blink = CCRepeatForever::create(
        (CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), 
        CCFadeIn::create(0.25f), 
        NULL) );

    cursor->runAction(blink);*/

    texture->release();

    free(pixels);

    return cursor;
}

void nodes_utility::changeNodeParent(Node* target, Node* newParent)
{
    if (target->getParent() == newParent)
        return;

    target->retain();
    
    target->removeFromParentAndCleanup(false);
    newParent->addChild(target);
    
    target->release();
}

bool nodes_utility::checkVisibility(Node* theNode)
{
    // CC_ASSERT(theNode != NULL);
    bool visible = false;
    for(Node* ptr = theNode; (ptr != nullptr && (visible = ptr->isVisible()) ); ptr = ptr->getParent())
        ;
    return visible;
}

Rect nodes_utility::getNodeRectInWorld(Node * pNode)
{
    Size contentSize = pNode->getContentSize();
    Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
    return RectApplyTransform(rect, pNode->getNodeToWorldTransform());
}

Rect nodes_utility::getNodeRect(Node * pNode)
{
    Rect rc;
    rc.size = pNode->getContentSize();
    return rc;
}

Rect nodes_utility::getNodeRectAR(Node* pNode)
{
    Rect rc;
    rc.size = pNode->getContentSize();
    rc.origin -= pNode->getAnchorPointInPoints();
    return rc;
}

bool nodes_utility::containsTouchPoint(cocos2d::Node* target, cocos2d::Touch* touch)
{
    assert(target != nullptr);

    //方案一  
    //将点击点转换成自己坐标系中的坐标，相对于0,0点  
    cocos2d::Point pt = target->convertTouchToNodeSpace(touch);

    const cocos2d::Size& size = target->getContentSize();

    cocos2d::Rect rc(0, 0, size.width, size.height);

    bool contains = (rc.containsPoint(pt));

    // CCLOG("check %#x coordinate:(%f, %f), contains:%d", target, pt.x, pt.y, contains);
    return contains;
}

bool nodes_utility::containsTouchPointAR(cocos2d::Node* target, cocos2d::Touch* touch)
{
    assert(target != nullptr);

    // 方案二
    // 将点击点转换成自己坐标系中的坐标，相对于锚点
    cocos2d::Point ptAR = target->convertTouchToNodeSpaceAR(touch);
    const cocos2d::Point& ap = target->getAnchorPoint();
    const cocos2d::Size& size = target->getContentSize();
    cocos2d::Rect rcar(-size.width * ap.x, -size.height * ap.y, size.width, size.height);
    return rcar.containsPoint(ptAR);
}

bool nodes_utility::containsTouchPointPR(Node* target, cocos2d::Touch* touch)
{
    assert(target != nullptr);
    // Point touchPoint = target->getParent()->convertTouchToNodeSpace(touch);

    return target->boundingBox().containsPoint(target->getParent()->convertTouchToNodeSpace(touch));
}

cocos2d::Rect nodes_utility::unreverseRect(const cocos2d::Rect& rect)
{
    if (rect.size.width < 0 && rect.size.height < 0)
    {
        return cocos2d::Rect(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height,
            -rect.size.width, -rect.size.height);
    }
    else if (rect.size.width < 0 && rect.size.height > 0)
    {
        return cocos2d::Rect(rect.origin.x + rect.size.width, rect.origin.y,
            -rect.size.width, rect.size.height);
    }
    else if (rect.size.height < 0 && rect.size.width > 0)
    {
        return cocos2d::Rect(rect.origin.x, rect.origin.y + rect.size.height,
            rect.size.width, -rect.size.height);
    }
    else {
        return rect;
    }
}


cocos2d::Image* nodes_utility::snapshot(Node* startNode, float scale)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    Director::getInstance()->setNextDeltaTimeZero(true);
    
    RenderTexture* rtx = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    // rtx->setKeepMatrix(true);
    cocos2d::Scene* scene = Director::getInstance()->getRunningScene();
    cocos2d::Point savedPos = scene->getPosition();
    scene->setPosition(-visibleOrigin);
    rtx->begin(); // rtx->beginWithClear(0, 0, 0, 0);
    scene->visit();
    rtx->end();
    scene->setPosition(savedPos);
    
    cocos2d::Size size = startNode->getContentSize();
    cocos2d::Point delta = ccp(nodes_layout::getNodeLeft(startNode) - visibleOrigin.x, nodes_layout::getNodeBottom(startNode) - visibleOrigin.y);
    
    cocos2d::Rect finalRect = cocos2d::Rect(delta.x, delta.y, size.width, size.height);
    Sprite *sprite = Sprite::createWithTexture(rtx->getSprite()->getTexture(), finalRect);
    sprite->setAnchorPoint(cocos2d::Point(0, 0));
    sprite->setFlipY(true);
    
    RenderTexture *finalRtx = RenderTexture::create(size.width * scale, size.height * scale, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    sprite->setScale(scale); // or use finalRtx->setKeepMatrix(true);
    finalRtx->begin();  // finalRtx->beginWithClear(0, 0, 0, 0); 
    sprite->visit();
    finalRtx->end();

    Director::getInstance()->getRenderer()->render();
    
    return finalRtx->newImage();
}

cocos2d::Image* nodes_utility::snapshot(float scale)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    Director::getInstance()->setNextDeltaTimeZero(true);

    RenderTexture* finalRtx = nullptr;
    
    RenderTexture* rtx = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    // rtx->setKeepMatrix(true);
    cocos2d::Scene* scene = Director::getInstance()->getRunningScene();
    cocos2d::Point savedPos = scene->getPosition();
    scene->setPosition(-visibleOrigin);
    rtx->begin(); // rtx->beginWithClear(0, 0, 0, 0); 
    Director::getInstance()->getRunningScene()->visit();
    rtx->end();
    scene->setPosition(savedPos);
    
    if (std::abs(scale - 1.0f) < 1e-6/* no scale */)
        finalRtx = rtx;
    else {

        /* scale */
        cocos2d::Rect finalRect = cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height);
        Sprite *sprite = Sprite::createWithTexture(rtx->getSprite()->getTexture(), finalRect);
        sprite->setAnchorPoint(cocos2d::Point(0, 0));
        sprite->setFlipY(true);

        finalRtx = RenderTexture::create(visibleSize.width * scale, visibleSize.height * scale, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
        
        sprite->setScale(scale); // or use finalRtx->setKeepMatrix(true);
        finalRtx->begin(); // finalRtx->beginWithClear(0, 0, 0, 0); 
        sprite->visit();
        finalRtx->end();
    }

    Director::getInstance()->getRenderer()->render();

    return finalRtx->newImage();
}

cocos2d::Image* nodes_utility::snapshot_v2(float scale)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point visibleOrigin = Director::getInstance()->getVisibleOrigin();

    Director::getInstance()->setNextDeltaTimeZero(true);

    RenderTexture* finalRtx = RenderTexture::create(visibleSize.width * scale, visibleSize.height * scale, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    finalRtx->setKeepMatrix(true);
    cocos2d::Scene* scene = Director::getInstance()->getRunningScene();
    cocos2d::Point savedPos = scene->getPosition();
    scene->setPosition(-visibleOrigin);
    finalRtx->begin(); // rtx->beginWithClear(0, 0, 0, 0); 
    Director::getInstance()->getRunningScene()->visit();
    finalRtx->end();
    scene->setPosition(savedPos);

    Director::getInstance()->getRenderer()->render();

    return finalRtx->newImage();
}



void nodes_utility::snapshot(const std::string& fileName, Node* startNode, float scale, const std::function<void(cocos2d::Image* image)>& callback)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    Director::getInstance()->setNextDeltaTimeZero(true);
    
    RenderTexture* rtx = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    cocos2d::Scene* scene = Director::getInstance()->getRunningScene();
    cocos2d::Point savedPos = scene->getPosition();
    scene->setPosition(-visibleOrigin);
    rtx->begin();
    scene->visit();
    rtx->end();
    scene->setPosition(savedPos);
    
    cocos2d::Size size = startNode->getContentSize();
    cocos2d::Point delta = ccp(nodes_layout::getNodeLeft(startNode) - visibleOrigin.x, nodes_layout::getNodeBottom(startNode) - visibleOrigin.y);
    
    cocos2d::Rect finalRect = cocos2d::Rect(delta.x, delta.y, size.width, size.height);
    Sprite *sprite = Sprite::createWithTexture(rtx->getSprite()->getTexture(), finalRect);
    sprite->setAnchorPoint(cocos2d::Point(0, 0));
    sprite->setFlipY(true);
    
    RenderTexture *finalRtx = RenderTexture::create(size.width * scale, size.height * scale);
    sprite->setScale(scale);
    finalRtx->begin();
    sprite->visit();
    finalRtx->end();
    
    /// Gets snapshot Image
    cocos2d::CustomCommand* command = new cocos2d::CustomCommand();
    command->init(finalRtx->getGlobalZOrder());
    command->func = [callback, finalRtx, command, fileName]{
        auto image = finalRtx->newImage();
        if (!fileName.empty())
            image->saveToFile(fileName);
        if (callback)
            callback(image);
        image->release();

        delete command;
        // finalRtx->release(); will destructed by command->func
    };

    finalRtx->retain();
    Director::getInstance()->getRenderer()->addCommand(command);
}

void nodes_utility::snapshot(const std::string& fileName, float scale, const std::function<void(cocos2d::Image* image)>& callback)
{
    RenderTexture* finalRtx = nullptr;
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    Director::getInstance()->setNextDeltaTimeZero(true);
    
    RenderTexture* rtx = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);

    cocos2d::Scene* scene = Director::getInstance()->getRunningScene();
    cocos2d::Point savedPos = scene->getPosition();
    scene->setPosition(-visibleOrigin);
    rtx->begin();
    Director::getInstance()->getRunningScene()->visit();
    rtx->end();
    scene->setPosition(savedPos);
    
    if (std::abs(scale - 1.0f) < 1e-6/* no scale */)
        finalRtx = rtx;
    else {
        /* scale */
        cocos2d::Rect finalRect = cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height);
        Sprite *sprite = Sprite::createWithTexture(rtx->getSprite()->getTexture(), finalRect);
        sprite->setAnchorPoint(cocos2d::Point(0, 0));
        sprite->setFlipY(true);

        finalRtx = RenderTexture::create(visibleSize.width * scale, visibleSize.height * scale,  Texture2D::PixelFormat::RGBA8888);
        sprite->setScale(scale);
        finalRtx->begin();
        sprite->visit();
        finalRtx->end();
    }

    /// Gets snapshot Image
    cocos2d::CustomCommand* command = new cocos2d::CustomCommand();
    command->init(finalRtx->getGlobalZOrder());
    command->func = [callback, finalRtx, command, fileName]{
        auto image = finalRtx->newImage();
        if (!fileName.empty())
            image->saveToFile(fileName);
        if (callback)
            callback(image);
        image->release();

        delete command;
        // finalRtx->release(); will destructed by command->func
    };

    finalRtx->retain();
    Director::getInstance()->getRenderer()->addCommand(command);
}
