//
// Copyright (c) 2014 purelib - All Rights Reserved
//

#ifndef _NX_InputBox_H_
#define _NX_InputBox_H_

#include "NXGameDefs.h"

namespace purelib {

// class TextFieldTTF;

/**
 * @addtogroup input
 * @{
 */

//class CC_DLL TextFieldDelegate
//{
//public:
//    virtual ~TextFieldDelegate() {}
//
//    /**
//    @brief    If the sender doesn't want to attach to the IME, return true;
//    */
//    virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender)
//    {
//        CC_UNUSED_PARAM(sender);
//        return false;
//    }
//
//    /**
//    @brief    If the sender doesn't want to detach from the IME, return true;
//    */
//    virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender)
//    {
//        CC_UNUSED_PARAM(sender);
//        return false;
//    }
//
//    /**
//    @brief    If the sender doesn't want to insert the text, return true;
//    */
//    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen)
//    {
//        CC_UNUSED_PARAM(sender);
//        CC_UNUSED_PARAM(text);
//        CC_UNUSED_PARAM(nLen);
//        return false;
//    }
//
//    /**
//    @brief    If the sender doesn't want to delete the delText, return true;
//    */
//    virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen)
//    {
//        CC_UNUSED_PARAM(sender);
//        CC_UNUSED_PARAM(delText);
//        CC_UNUSED_PARAM(nLen);
//        return false;
//    }
//
//    /**
//    @brief    If the sender doesn't want to draw, return true.
//    */
//    virtual bool onVisit(TextFieldTTF * sender,Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
//    {
//        CC_UNUSED_PARAM(sender);
//        return false;
//    }
//};

/**
@brief    A simple text input field with TTF font.
*/
class InputBox : public cocos2d::Node, public IMEDelegate
{
public:
    /**
     * @js ctor
     */
    InputBox();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~InputBox();

    //char * description();

    /** creates a TextFieldTTF from a fontname, alignment, dimension and font size */
    //static InputBox * create(const std::string& placeholder, const Size& dimensions, TextHAlignment alignment, const std::string& fontName, float fontSize, float cursorWidth = 6, const Color3B& cursorColor = Color3B::WHITE);
    /** creates a TextFieldTTF from a fontname and font size */
    static InputBox * create(const std::string& placeholder, const std::string& fontName, float fontSize, float cursorWidth = 6, const Color4B& color = Color4B::WHITE);
    /** initializes the TextFieldTTF with a font name, alignment, dimension and font size */
    //bool initWithPlaceHolder(const std::string& placeholder, const Size& dimensions, TextHAlignment alignment, const std::string& fontName, float fontSize, float cursorWidth = 6, const Color3B& cursorColor = Color3B::WHITE);
    /** initializes the TextFieldTTF with a font name and font size */
    bool initWithPlaceHolder(const std::string& placeholder, const std::string& fontName, float fontSize, float cursorWidth = 6, const Color4B& color = Color4B::WHITE);

    void enableIME(Node* control);
    void disableIME(void);

    inline int                getCharCount() const { return charCount; };

    virtual void              setColorSpaceHolder(const Color4B& color);
    virtual const Color4B&    getColorSpaceHolder() const;
    
    virtual void              setTextColor(const Color4B& textColor);
    virtual const Color4B&    getTextColor(void) const;

    void                      setCursorColor(const Color3B& color);
    const Color3B&            getCursorColor(void) const;

    // input text property
    virtual void               setString(const std::string& text);
    virtual const std::string& getString() const;

    // place holder text property
    // place holder text displayed when there is no text in the text field.
    virtual void               setPlaceHolder(const std::string& text);
    virtual const std::string& getPlaceHolder(void) const;

    virtual void         setSecureTextEntry(bool value);
    virtual bool         isSecureTextEntry() const;

    /*virtual void         visit(Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated) override;*/

    bool                 empty(void) const { return this->charCount == 0 || this->inputText.empty();}
    
    void                 setModifyCallback(const std::function<void(void)>& callback);

    virtual const Size&  getContentSize() const override;

    virtual void         setEnabled(bool bEnabled);
    virtual bool         isEnabled(void) const;

    void                 setEditable(bool bEditable){ editable = bEditable; }
    bool                 isEditable(void) const { return editable; }


    /// fonts
    void setTextFontSize(float size);
    float getTextFontSize() const;
    void setTextFontName(const std::string& fontName);
    const std::string& getTextFontName() const;

    CC_SYNTHESIZE(size_t, charLimit, CharLimit);

    bool isSystemFont(void) const{ return systemFontUsed; }

protected:
    //////////////////////////////////////////////////////////////////////////
    // IMEDelegate interface
    //////////////////////////////////////////////////////////////////////////
    void openIME(void);
    void closeIME(void);

    virtual bool canAttachWithIME() override;
    virtual bool canDetachWithIME() override;
    virtual void insertText(const char * text, size_t len) override;
    virtual void deleteBackward() override;
    virtual const std::string& getContentText() override;
    /**
    @brief    Open keyboard and receive input text.
    */
    virtual bool attachWithIME();

    /**
    @brief    End text input and close keyboard.
    */
    virtual bool detachWithIME();

    void         updateContentSize(void);
    
    void         __initCursor(int height, int width = 6, const Color4B& color = Color4B::WHITE);
    void         __showCursor(void);
    void         __hideCursor(void);
    void         __moveCursor(void);

protected:

    bool                        systemFontUsed;
    std::string                 fontName;
    float                       fontSize;

    bool                        editable;

    Label*                      renderLabel;
    
    size_t                      charCount;
    std::string                 inputText;

    std::string                 placeHolder;
    Color4B                     colorSpaceHolder;
    Color4B                     colorText;

    bool                        secureTextEntry;

    Sprite*                   cursor;
    
    bool                        enabled;

    EventListenerTouchOneByOne* listener;

    std::function<void(void)>   modifyCallback; // Content change callback
};

// end of input group
/// @}

};

#endif    // _INS_InputBox_H_
