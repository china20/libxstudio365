#include <limits>
#include "NXNodesLayout.h"
#include "NXFloatingText.h"
#include "NXSingleton.h"
#include "utils/oslib.h"

static std::string lgFontName = "Courier New";
static float lgFontSize = 36;

namespace purelib {

enum {
    kTagFloatingText = 113011,
};

void setFloatingFontName(const char* fontName)
{
    lgFontName = fontName;
}

void setFloatingFontSize(float size)
{
    lgFontSize = size;
}

bool FloatingText::init(void)
{
    if (FileUtils::getInstance()->isFileExist(lgFontName.c_str()))
    {
        TTFConfig ttfConfig(lgFontName.c_str(), lgFontSize, GlyphCollection::DYNAMIC);
        if (this->setTTFConfig(ttfConfig))
        {
            this->setString("Empty");
        }
    }
    else {
        this->setSystemFontName(lgFontName);
        this->setSystemFontSize(lgFontSize);
        // this->setDimensions(dimensions.width, dimensions.height);
        this->setString("Empty");
    }

    this->setZOrder((std::numeric_limits<int>::max)());
    this->setAnchorPoint(cocos2d::Point(.5f, .5f));
    this->ignoreAnchorPointForPosition(false);
    this->setHorizontalAlignment(kCCTextAlignmentCenter);
    this->setVerticalAlignment(kCCVerticalTextAlignmentCenter);

    this->enableOutline(ccc4(0, 0, 0, 128), 2);
    this->enableShadow(ccc4(255, 0, 0, 128));

    this->setTag(kTagFloatingText);

    return true;
}

void FloatingText::setText(const char* text, const ccColor3B& color)
{
    this->setColor(color);
    this->setString(text);
}

void FloatingText::showMessage(int type, const char* message, float delay)
{
    CCLOG("FloatingText::showMessage:type:%d, msg:%s,delay time:%f", type, message, delay);
    this->delayShowType = type;
    this->delayShowMessage = message;

	cocos2d::Node* helper = nullptr;// cocos2d::Singleton<cocos2d::Node>::instance();
    if(helper->getParent() == nullptr) {
        Director::getInstance()->getRunningScene()->addChild(helper);
    }

    Action* delayWorker = CCSequence::create(
        CCDelayTime::create(delay),
        CCCallFunc::create(this, callfunc_selector(FloatingText::doMessageShowing)),
        NULL
        );
    helper->runAction(delayWorker);
}

void FloatingText::doMessageShowing(void)
{
    CCLOG("FloatingText::doMessageShowing:%s", this->delayShowMessage.c_str());
    switch(this->delayShowType)
    {
    case kFloatingTextNormal:
        createTextTagForNormal("%s", this->delayShowMessage.c_str());
        break;
    case kFloatingTextWarnning:
        createTextTagForWarnning("%s", this->delayShowMessage.c_str());
        break;
    case kFloatingTextError:
        createTextTagForError("%s", this->delayShowMessage.c_str());
        break;
    }
}

#define MAX_LEN         (255 + 1)
void createTextTagForWarnning(const char* pszFormat, ...)
{
    char szBuf[MAX_LEN];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, MAX_LEN, pszFormat, ap);
    va_end(ap);

	createTextTag(szBuf, 0.0f, Color3B::YELLOW);
}

void createTextTagForError(const char* pszFormat, ...)
{
    char szBuf[MAX_LEN];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, MAX_LEN, pszFormat, ap);
    va_end(ap);

	createTextTag(szBuf, 0.0f, Color3B::RED);
}

void createTextTagForNormal(const char* pszFormat, ...)
{
    char szBuf[MAX_LEN];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, MAX_LEN, pszFormat, ap);
    va_end(ap);

    createTextTag(szBuf, 0.0f, ccc3(0, 255, 255));
}

void createTextTagWithDuration(const char* pszFormat, float duration, ...)
{
    char szBuf[MAX_LEN];

    va_list ap;
    va_start(ap, duration);
    vsnprintf(szBuf, MAX_LEN, pszFormat, ap);
    va_end(ap);

    createTextTag(szBuf, duration);
}

void createTextTag(const char* text, float duration, const ccColor3B& color)
{
    auto runningScene = Director::getInstance()->getRunningScene();
    auto theFloating = dynamic_cast<FloatingText*>( runningScene->getChildByTag(kTagFloatingText) ) ;
    if(_IsNull( theFloating ) )
    {
        theFloating = new FloatingText();
        theFloating->init();
        runningScene->addChild(theFloating);

        theFloating->release();
    }

    theFloating->stopAllActions();
    theFloating->setVisible(false);
    theFloating->setText(text, color);

    nodes_layout::centerNode(theFloating);

    Action* action = CCSequence::create(
        CCShow::create(),
        CCSpawn::create(
        CCFadeIn::create(0.5f),
        CCMoveBy::create(1.0 + duration, cocos2d::Point(0, 50)),
        CCSequence::create(
        CCDelayTime::create(2.0f + duration),
        CCSpawn::create(CCFadeOut::create(1.0f), CCMoveBy::create(1.0, cocos2d::Point(0, 50)),
        nullptr), 
        nullptr), 
        nullptr),

        /*CCCallFunc::create(theFloating, callfunc_selector(XGFloatingText::callbackRemoveNodeWhenDidAction)),*/ 

        nullptr);

    theFloating->runAction(action);
    // theFloating->getLabel()->runAction(action);
}

void showTextTag(const char* text, const ccColor3B& color)
{
    auto runningScene = Director::getInstance()->getRunningScene();
    auto theFloating = dynamic_cast<FloatingText*>( runningScene->getChildByTag(kTagFloatingText) ) ;
    if(_IsNull( theFloating ) )
    {
        theFloating = new FloatingText();
        theFloating->init();
        runningScene->addChild(theFloating);

        theFloating->release();
    }

    theFloating->stopAllActions();

    theFloating->setVisible(false);
    theFloating->setText(text, color);
    nodes_layout::centerNode(theFloating);
    theFloating->setVisible(true);
}

void hideTextTag(void)
{
    auto theFloating = dynamic_cast<FloatingText*>( Director::getInstance()->getRunningScene()->getChildByTag(kTagFloatingText) ) ;
    if(_IsNotNull( theFloating ) )
    {
        theFloating->stopAllActions();
        theFloating->setVisible(false);
    }
}

};


