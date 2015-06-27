// Copyright (c) 2014 purelib Corporation. All rights reserved.
#ifndef _INS_FLOATINGTEXT_H_
#define _INS_FLOATINGTEXT_H_
#include <cocos2d.h>
#include "NXColorUtility.h"
USING_NS_CC;
namespace purelib {
enum {
    kFloatingTextNormal,
    kFloatingTextWarnning,
    kFloatingTextError,
};

class FloatingText : public Label
{
public:
    CREATE_FUNC(FloatingText);

    bool init(void);

    void setText(const char* text, const Color3B& color = Color3B::WHITE);
    
    void showMessage(int type, const char* message, float delay);

    void doMessageShowing(void);

    int         delayShowType;
    std::string delayShowMessage;
};

extern void setTextTagFont(const char* fontName, float fontSize);
extern void createTextTagForWarnning(const char* text, ...);
extern void createTextTagForError(const char* text, ...);
extern void createTextTagForNormal(const char* text, ...);
extern void createTextTagWithDuration(const char* text, float duration, ...);
extern void createTextTag(const char* text, float duration = 0.0f, const Color3B& color = Color3B(0, 255, 255));

extern void showTextTag(const char* text, const Color3B& color = Color3B(0, 255, 255));
extern void hideTextTag(void);

};

#endif

