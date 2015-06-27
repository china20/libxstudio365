#include "NXShaderSprite.h"

NS_NX_BEGIN

static const char* grayShader = 
"#ifdef GL_ES\n\
precision mediump float;\n\
#endif\n\
uniform sampler2D u_texture;\n\
varying vec2 v_texCoord;\n\
varying vec4 v_fragmentColor;\n\
void main(void)\n\
{\n\
// Convert to greyscale using NTSC weightings\n\
float alpha = texture2D(u_texture, v_texCoord).a;\n\
vec4 col = texture2D(u_texture, v_texCoord); \n \
float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
gl_FragColor = vec4(grey, grey, grey, col.a); \n \
}\n";

static const char* transparentShader = " \n\
#ifdef GL_ES                                            \n\
precision lowp float;                                   \n\
#endif                                                  \n\
varying vec4 v_fragmentColor;                           \n\
varying vec2 v_texCoord;                                \n\
uniform sampler2D u_texture;                            \n\
void main()                                             \n\
{                                                       \n\
    float ratio=0.0;                                    \n\
    vec4 texColor = texture2D(u_texture, v_texCoord);   \n\
    ratio = texColor[0] > texColor[1]?(texColor[0] > texColor[2] ? texColor[0] : texColor[2]) :(texColor[1] > texColor[2]? texColor[1] : texColor[2]);                                      \n\
if (ratio != 0.0)                                          \n\
{                                                          \n\
    texColor[0] = texColor[0] /  ratio;                    \n\
    texColor[1] = texColor[1] /  ratio;                    \n\
    texColor[2] = texColor[2] /  ratio;                    \n\
    texColor[3] = ratio;                                   \n\
}                                                          \n\
else                                                       \n\
{                                                          \n\
    texColor[3] = 0.0;                                     \n\
}                                                          \n\
gl_FragColor = v_fragmentColor*texColor;                   \n\
}";


ShaderSprite* ShaderSprite::create(const char* pszFileName)
{
    ShaderSprite *pRet = new(std::nothrow) ShaderSprite();
    if (pRet && pRet->initWithFile(pszFileName)) {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ShaderSprite::initWithTexture(CCTexture2D *pTexture, const cocos2d::Rect& rect)
{
    do{
        //        CCLog("override initWithTexture!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, rect));

        // 加载顶点着色器和片元着色器
        auto shaderProgram = new CCGLProgram();
        shaderProgram ->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, grayShader);

        CHECK_GL_ERROR_DEBUG();

        // 启用顶点着色器的attribute变量，坐标、纹理坐标、颜色
        shaderProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        shaderProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        shaderProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

        CHECK_GL_ERROR_DEBUG();

        // 自定义着色器链接
        shaderProgram->link();

        CHECK_GL_ERROR_DEBUG();

        // 设置移动、缩放、旋转矩阵
        shaderProgram->updateUniforms();

        CHECK_GL_ERROR_DEBUG();

        this->setShaderProgram(shaderProgram);

        shaderProgram->release();

        return true;

    }while(0);
    return false;
}

void ShaderSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");

    // CCAssert(, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");

    CC_NODE_DRAW_SETUP();

    //
    // 启用attributes变量输入，顶点坐标，纹理坐标，颜色
    //
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
    ccGLBlendFunc(this->getBlendFunc().src, this->getBlendFunc().dst);

    this->getShaderProgram()->use();
    this->getShaderProgram()->setUniformsForBuiltins();

    // 绑定纹理到纹理槽0
    ccGLBindTexture2D(this->getTexture()->getName());



#define kQuadSize sizeof(this->_quad.bl)
    long offset = (long)&this->_quad;

    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    CHECK_GL_ERROR_DEBUG();

    CC_INCREMENT_GL_DRAWS(1);
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}

NS_NX_END;

