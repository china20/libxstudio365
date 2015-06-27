#ifndef _INS_SHADERSPRITE_H_
#define _INS_SHADERSPRITE_H_
/****************************************************************************
Copyright (c) 2014 purelib Corporation. All rights reserved.
http://www.purelib.com
****************************************************************************/
#include "NXLibDefs.h"

NS_NX_BEGIN

enum SpriteShaderType
{
	kSpriteShaderGray = 0,
	kSpriteShaderTransparent,
};

class ShaderSprite : public Sprite 
{    
public:
    static ShaderSprite* create(const char* pszFileName);
    virtual bool initWithTexture(Texture2D *pTexture, const cocos2d::Rect& rect);
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
};

NS_NX_END;

#endif

