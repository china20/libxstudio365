//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#include "NXInputBox.h"

#include "base/CCDirector.h"
#include "CCGLView.h"

namespace purelib {

static int _calcCharCount(const char * text)
{
    int n = 0;
    char ch = 0;
    while ((ch = *text))
    {
        CC_BREAK_IF(!ch);

        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++text;
    }
    return n;
}

//////////////////////////////////////////////////////////////////////////
// constructor and destructor
//////////////////////////////////////////////////////////////////////////

InputBox::InputBox()
    : editable(true)
    , renderLabel(nullptr)
    , charCount(0)
    , inputText("")
    , placeHolder("")
    , colorText(Color4B::WHITE)
    , colorSpaceHolder(Color4B::GRAY)
    , secureTextEntry(false)
    , cursor(nullptr)
    , enabled(true)
    , listener(nullptr)
    , modifyCallback(nullptr)
{
}

InputBox::~InputBox()
{
    if (this->listener)
        CCEVENTMGR->removeEventListener(this->listener);
}

//////////////////////////////////////////////////////////////////////////
// static constructor
//////////////////////////////////////////////////////////////////////////
InputBox * InputBox::create(const std::string& placeholder, const std::string& fontName, float fontSize, float cursorWidth, const Color4B& cursorColor)
{
    InputBox *ret = new InputBox();
    if (ret && ret->initWithPlaceHolder("", fontName, fontSize, cursorWidth, cursorColor))
    {
        ret->autorelease();
        if (placeholder.size() > 0)
        {
            ret->setPlaceHolder(placeholder);
        }
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// initialize
//////////////////////////////////////////////////////////////////////////
bool InputBox::initWithPlaceHolder(const std::string& placeholder, const std::string& fontName, float fontSize, float cursorWidth, const Color4B& cursorColor)
{
    this->placeHolder = placeholder;

    this->renderLabel = Label::create(placeholder, fontName, fontSize, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    this->renderLabel->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    this->addChild(this->renderLabel);
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);

    __initCursor(fontSize, cursorWidth, cursorColor);

    this->fontSize = fontSize;
    this->systemFontUsed = !FileUtils::getInstance()->isFileExist(fontName);

    return true;
}

const std::string& InputBox::getTextFontName() const
{
    return this->fontName;
}

void InputBox::setTextFontName(const std::string& fontName)
{
    if (FileUtils::getInstance()->isFileExist(fontName))
    {
        TTFConfig config = renderLabel->getTTFConfig();
        config.fontFilePath = fontName;
        config.fontSize = this->fontSize;
        renderLabel->setTTFConfig(config);
        systemFontUsed = false;
    }
    else
    {
        renderLabel->setSystemFontName(fontName);
        if (!systemFontUsed)
        {
            renderLabel->requestSystemFontRefresh();
        }
        renderLabel->setSystemFontSize(this->fontSize);
        systemFontUsed = true;
    }
    this->fontName = fontName;

    this->updateContentSize();
}

void InputBox::setTextFontSize(float size)
{
    if (this->systemFontUsed)
    {
        renderLabel->setSystemFontSize(size);
    }
    else
    {
        TTFConfig config = renderLabel->getTTFConfig();
        config.fontSize = size;
        renderLabel->setTTFConfig(config);
    }

    this->fontSize = size;
    this->updateContentSize();
}

float InputBox::getTextFontSize() const
{
    return this->fontSize;
}

void InputBox::enableIME(Node* control)
{
    listener = EventListenerTouchOneByOne::create();
    
    if(control == nullptr)
        control = this;

    listener->onTouchBegan = [](Touch*, Event*){ return true; };
    listener->onTouchEnded = [control,this](Touch* touch, Event* e) {

        if(!nodes_utility::checkVisibility(this) || !this->editable || !this->enabled)
            return;

        Rect rect;
        auto point = control->convertTouchToNodeSpace(touch); // touch->getLocation();
        CCLOG("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);

        rect = nodes_utility::getNodeRect(control); // Size: (526, 101)
        CCLOG("KeyboardNotificationLayer:TrackNode at(origin:%f,%f, size:%f,%f)",
            rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        if(rect.containsPoint(point))
        {
            this->openIME();
        }
        else {
            this->closeIME();
        }
    };

    CCEVENTMGR->addEventListenerWithSceneGraphPriority(listener, this);
}

void InputBox::disableIME(void)
{
    CCEVENTMGR->removeEventListener(listener);

    listener = nullptr;
    closeIME();
}

//////////////////////////////////////////////////////////////////////////
// IMEDelegate
//////////////////////////////////////////////////////////////////////////

bool InputBox::attachWithIME()
{
    bool ret = IMEDelegate::attachWithIME();
    if (ret)
    {
        // open keyboard
        GLView * pGlView = Director::getInstance()->getOpenGLView();
        if (pGlView)
        {
            pGlView->setIMEKeyboardState(true);
        }
    }
    return ret;
}

bool InputBox::detachWithIME()
{
    bool ret = IMEDelegate::detachWithIME();
    if (ret)
    {
        // close keyboard
        GLView * glView = Director::getInstance()->getOpenGLView();
        if (glView)
        {
            glView->setIMEKeyboardState(false);
        }
    }
    return ret;
}

void InputBox::openIME(void)
{
    CCLOG("InputBox:: openIME");
    this->attachWithIME();
    __showCursor();
}

void InputBox::closeIME(void)
{
    CCLOG("InputBox:: closeIME");
    __hideCursor();
    this->detachWithIME();   
}

bool InputBox::canAttachWithIME()
{
    return true;//(_delegate) ? (! _delegate->onTextFieldAttachWithIME(this)) : true;
}

bool InputBox::canDetachWithIME()
{
    return true;//(_delegate) ? (! _delegate->onTextFieldDetachWithIME(this)) : true;
}

void InputBox::insertText(const char * text, size_t len)
{
    if(this->charCount >= this->charLimit || !this->editable || !this->enabled)
    {
        return;
    }

    std::string insert(text, (std::min)( this->charLimit - this->charCount, len) );
 
    // insert \n means input end
    auto pos = insert.find('\n');
    if (insert.npos != pos)
    {
        len = pos;
        insert.erase(pos);
    }

    if (len > 0)
    {
        //if (_delegate && _delegate->onTextFieldInsertText(this, insert.c_str(), len))
        //{
        //    // delegate doesn't want to insert text
        //    return;
        //}

        charCount += _calcCharCount(insert.c_str());
        std::string sText(inputText);
        sText.append(insert);
        this->setString(sText);

        // this->contentDirty = true;
        __moveCursor();

        if(this->modifyCallback)
            this->modifyCallback();
    }

    if ( insert.npos == pos) {
        return;
    }

    // '\n' inserted, let delegate process first
    /*if (_delegate && _delegate->onTextFieldInsertText(this, "\n", 1))
    {
        return;
    }*/

    // if delegate hasn't processed, detach from IME by default
    this->closeIME();
}

void InputBox::deleteBackward()
{
    if (!this->editable || !this->enabled || 0 == this->charCount) {
#ifdef _WIN32
        MessageBeep(0);
#endif
        return;
    }

    size_t len = inputText.length();
    if (0 == len)
    {
#ifdef _WIN32
        MessageBeep(0);
#endif
        // there is no string
        // __moveCursor();
        return;
    }
    
    // get the delete byte number
    size_t deleteLen = 1;    // default, erase 1 byte

    while(0x80 == (0xC0 & inputText.at(len - deleteLen)))
    {
        ++deleteLen;
    }

    //if (_delegate && _delegate->onTextFieldDeleteBackward(this, _inputText.c_str() + len - deleteLen, static_cast<int>(deleteLen)))
    //{
    //    // delegate doesn't wan't to delete backwards
    //    return;
    //}

    // if all text deleted, show placeholder string
    if (len <= deleteLen)
    {
        this->inputText.clear();
        this->charCount = 0;
        this->renderLabel->setTextColor(colorSpaceHolder);
        this->renderLabel->setString(placeHolder);

        __moveCursor();

        // this->contentDirty = true;

        if(this->modifyCallback)
            this->modifyCallback();
        return;
    }

    // set new input text
    std::string text(inputText.c_str(), len - deleteLen);
    this->setString(text);

    __moveCursor();

    if(this->modifyCallback)
        this->modifyCallback();
}

const std::string& InputBox::getContentText()
{
    return inputText;
}

void InputBox::setTextColor(const Color4B &color)
{
    colorText = color;
    if (!this->inputText.empty())
        this->renderLabel->setTextColor(colorText);
}

const Color4B&  InputBox::getTextColor(void) const
{
    return colorText;
}

void InputBox::setCursorColor(const Color3B& color)
{
    this->cursor->setColor(color);
}

const Color3B& InputBox::getCursorColor(void) const
{
    return this->cursor->getColor();
}

//void InputBox::visit(Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated)
//{
//   /*if (contentDirty)
//    {
//        this->setContentSize(renderLabel->getContentSize());
//        contentDirty = false;
//    }*/
//    Node::visit(renderer,parentTransform,parentTransformUpdated);
//}

const Color4B& InputBox::getColorSpaceHolder() const
{
    return colorSpaceHolder;
}

//void InputBox::setColorSpaceHolder(const Color3B& color)
//{
//    colorSpaceHolder.r = color.r;
//    colorSpaceHolder.g = color.g;
//    colorSpaceHolder.b = color.b;
//    colorSpaceHolder.a = 255;
//}

void InputBox::setColorSpaceHolder(const Color4B& color)
{
    colorSpaceHolder = color;
    if (this->inputText.empty())
        this->renderLabel->setTextColor(color);
}

//////////////////////////////////////////////////////////////////////////
// properties
//////////////////////////////////////////////////////////////////////////

// input text property
void InputBox::setString(const std::string &text)
{
    static char bulletString[] = {(char)0xe2, (char)0x80, (char)0xa2, (char)0x00};    

    this->inputText = text;

    std::string secureText;

    std::string* displayText = &this->inputText;

    if ( !this->inputText.empty() )
    {
        if (secureTextEntry)
        {
            size_t length = inputText.length();
            displayText = &secureText;

            while (length > 0)
            {
                displayText->append(bulletString);
                --length;
            }
        }
    }

    // if there is no input text, display placeholder instead
    if (this->inputText.empty())
    {
        renderLabel->setTextColor(colorSpaceHolder);
        renderLabel->setString(placeHolder);
    }
    else
    {
        renderLabel->setTextColor(colorText);
        renderLabel->setString(*displayText);
    }

    charCount = _calcCharCount(inputText.c_str());
}

void InputBox::updateContentSize(void)
{
    this->setContentSize(renderLabel->getContentSize());
}

const std::string& InputBox::getString() const
{
    return inputText;
}

// place holder text property
void InputBox::setPlaceHolder(const std::string& text)
{
    placeHolder = text;
    if (inputText.empty())
    {
        renderLabel->setTextColor(colorSpaceHolder);
        renderLabel->setString(placeHolder);
    }
}

const std::string& InputBox::getPlaceHolder() const
{
    return placeHolder;
}

// secureTextEntry
void InputBox::setSecureTextEntry(bool value)
{
    if (secureTextEntry != value)
    {
        secureTextEntry = value;
        this->setString(this->getString());
        __moveCursor();
    }
}

bool InputBox::isSecureTextEntry() const
{
    return secureTextEntry;
}

const Size& InputBox::getContentSize() const
{
    const_cast<InputBox*>(this)->setContentSize(renderLabel->getContentSize());
    return Node::getContentSize();
}

void  InputBox::setEnabled(bool bEnabled)
{
    if(this->enabled != bEnabled)
    {
        if(!bEnabled) {
            this->closeIME();
        }
        this->enabled = bEnabled;
    }
}

bool  InputBox::isEnabled(void) const
{
    return this->enabled;
}

void InputBox::__initCursor(int height, int width, const Color4B& color)
{
    this->cursor = nodes_utility::a_lump(Color4B(color), height, width);
    
    this->addChild(this->cursor);

    nodes_layout::setNodeLB(this->cursor, cocos2d::Point::ZERO);

    CCAction* blink = CCRepeatForever::create(
        (CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), 
        CCFadeIn::create(0.25f), 
        NULL) );

    this->cursor->runAction(blink);

    this->cursor->setVisible(false);

    __moveCursor();
}

void InputBox::__showCursor(void)
{
    if(_IsNotNull(this->cursor))
        this->cursor->setVisible(true);
}

void InputBox::__hideCursor(void)
{
    if(_IsNotNull(this->cursor))
        this->cursor->setVisible(false);
}

void InputBox::__moveCursor(void)
{
    if(_IsNotNull(this->cursor))
    {
        if(0 == this->getCharCount())
        {
            nodes_layout::setNodeLB(this->cursor, cocos2d::Point::ZERO);
        }
        else 
        {
            nodes_layout::setNodeLeft(this->cursor, this->getContentSize().width);
        }

        /*if(!this->cursor->isVisible())
            this->cursor->setVisible(true);*/
    }
}

};

