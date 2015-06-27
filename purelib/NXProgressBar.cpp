//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#include "NXProgressBar.h"
//#include "support/CCPointExtension.h"
#include "base/CCDirector.h"
//#include "extensions/cocos-ext.h"

//USING_NS_CC_EXT;

namespace purelib {

ProgressBar::ProgressBar()
: m_value(0.0f)
, m_minimumValue(0.0f)
, m_maximumValue(0.0f)
, m_minimumAllowedValue(0.0f)
, m_maximumAllowedValue(0.0f)
, m_progressSprite(NULL)
, m_backgroundSprite(NULL)
{

}

ProgressBar::~ProgressBar()
{
    CC_SAFE_RELEASE(m_progressSprite);
    CC_SAFE_RELEASE(m_backgroundSprite);
}

ProgressBar* ProgressBar::create(const char* bgFile, const char* progressFile)
{
    // Prepare background for slider
    Sprite *backgroundSprite      = Sprite::create(bgFile);
    
    // Prepare progress for slider
    Sprite *progressSprite        = Sprite::create(progressFile);
    
    
    return ProgressBar::create(backgroundSprite, progressSprite);
}

ProgressBar* ProgressBar::create(Sprite * backgroundSprite, Sprite* pogressSprite)
{
    ProgressBar *pRet = new ProgressBar();
    pRet->initWithSprites(backgroundSprite, pogressSprite);
    pRet->autorelease();
    return pRet;
}

bool ProgressBar::initWithFiles(const char * bgFile, const char* progressFile)
{
    // Prepare background for slider
    Sprite *backgroundSprite      = Sprite::create(bgFile);
    
    // Prepare progress for slider
    Sprite *progressSprite        = Sprite::create(progressFile);

    return this->initWithSprites(backgroundSprite, progressSprite);
}

 bool ProgressBar::initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite)
 {
     if (cocos2d::Layer::init())
     {
        CCAssert(backgroundSprite,  "Background sprite must be not nil");
        CCAssert(progressSprite,    "Progress sprite must be not nil");
        

        ignoreAnchorPointForPosition(false);
        // setTouchEnabled(true);

        this->setBackgroundSprite(backgroundSprite);
        this->setProgressSprite(progressSprite);

        // Defines the content size
        cocos2d::Rect maxRect   = ControlUtils::RectUnion(backgroundSprite->boundingBox(), progressSprite->boundingBox());

        setContentSize(CCSizeMake(maxRect.size.width, maxRect.size.height));
        
        // Add the slider background
        m_backgroundSprite->setAnchorPoint(ccp(0.5f, 0.5f));
        m_backgroundSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
        addChild(m_backgroundSprite);

        // Add the progress bar
        m_progressSprite->setAnchorPoint(ccp(0.0f, 0.5f));
        m_progressSprite->setPosition(ccp(0.0f, this->getContentSize().height / 2));
        addChild(m_progressSprite);

        // Add the slider thumb  
       /* m_thumbSprite->setPosition(ccp(0.0f, this->getContentSize().height / 2));
        addChild(m_thumbSprite);*/
        
        // Init default values
        m_minimumValue                   = 0.0f;
        m_maximumValue                   = 1.0f;

        setValue(m_minimumValue);
        return true;
     }
     else
     {
         return false;
     }
 }

 void ProgressBar::updateTexture(void)
 {
     // Defines the content size
     cocos2d::Rect maxRect = ControlUtils::RectUnion(m_backgroundSprite->boundingBox(), m_progressSprite->boundingBox());

     setContentSize(CCSizeMake(maxRect.size.width, maxRect.size.height));

     // Add the slider background
     m_backgroundSprite->setAnchorPoint(ccp(0.5f, 0.5f));
     m_backgroundSprite->setPosition(ccp(this->getContentSize().width / 2, this->getContentSize().height / 2));
     //addChild(m_backgroundSprite);

     // Add the progress bar
     m_progressSprite->setAnchorPoint(ccp(0.0f, 0.5f));
     m_progressSprite->setPosition(ccp(0.0f, this->getContentSize().height / 2));
     //addChild(m_progressSprite);

     needsLayout();
 }

 void ProgressBar::setValue(float value)
 {
     // set new value with sentinel
     if (value < m_minimumValue)
     {
         value = m_minimumValue;
     }

     if (value > m_maximumValue) 
     {
         value = m_maximumValue;
     }

     m_value = value;

     this->needsLayout();

     // this->sendActionsForControlEvents(Control::EventType::VALUE_CHANGED);
 }

 void ProgressBar::setMinimumValue(float minimumValue)
 {
     m_minimumValue=minimumValue;
     m_minimumAllowedValue = minimumValue;
     if (m_minimumValue >= m_maximumValue)    
     {
        m_maximumValue   = m_minimumValue + 1.0f;
     }
     setValue(m_value);
 }

 void ProgressBar::setMaximumValue(float maximumValue)
 {
     m_maximumValue=maximumValue;
     m_maximumAllowedValue = maximumValue;
     if (m_maximumValue <= m_minimumValue)   
     {
        m_minimumValue   = m_maximumValue - 1.0f;
     }
     setValue(m_value);
 }

void ProgressBar::needsLayout()
{
    if (NULL == m_backgroundSprite || NULL == m_progressSprite)
    {
        return;
    }
    // Update thumb position for new value
    float percent               = (m_value - m_minimumValue) / (m_maximumValue - m_minimumValue);

    /*CCPoint pos                 = m_thumbSprite->getPosition();
    pos.x                       = percent * m_backgroundSprite->getContentSize().width;
    m_thumbSprite->setPosition(pos);*/
    float x = percent * m_backgroundSprite->getContentSize().width;

    // Stretches content proportional to newLevel
    cocos2d::Rect textureRect          = m_progressSprite->getTextureRect();
    textureRect                 = CCRectMake(textureRect.origin.x, textureRect.origin.y, x, textureRect.size.height);
    m_progressSprite->setTextureRect(textureRect, m_progressSprite->isTextureRectRotated(), textureRect.size);
}

//float ProgressBar::valueForLocation(CCPoint location)
//{
//    float percent = location.x/ m_backgroundSprite->getContentSize().width;
//    return MAX(MIN(m_minimumValue + percent * (m_maximumValue - m_minimumValue), m_maximumAllowedValue), m_minimumAllowedValue);
//}

}
