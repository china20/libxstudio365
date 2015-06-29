//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#ifndef _VXENUMTYPEINFO_H_
#define _VXENUMTYPEINFO_H_
#include <string>
#include <unordered_map>
#include <cocos2d.h>
#include <cocos/ui/CocosGUI.h>

enum class VXLocateType : unsigned char
{
    LOCATE_FREE,
    LOCATE_LEFT_TOP,
    LOCATE_LEFT_BOTTOM,
    LOCATE_RIGHT_TOP,
    LOCATE_RIGHT_BOTTOM,
    LOCATE_CENTER,
    LOCATE_CENTER_LEFT,
    LOCATE_CENTER_TOP,
    LOCATE_CENTER_RIGHT,
    LOCATE_CENTER_BOTTOM,
};

//
// enum VXObjectType
//
enum VXObjectType
{
    kVXObjectTypeNone,

    // scene
    kVXObjectTypeScene,

    // base
    kVXObjectTypeNode,
    kVXObjectTypeSprite,
    kVXObjectTypeLayer, // unused
    kVXObjectTypeLayerColor,
    kVXObjectTypeScale9Sprite,
    kVXObjectTypeLabel,
    kVXObjectTypeProgressBar,
    kVXObjectTypeParticleSystem,
    kVXObjectTypeLabelAtlas,
    kVXObjectTypeLabelBMFont,
    kVXObjectTypeRichLabel,
    
    // control
    kVXObjectTypeTextField,
    kVXObjectTypeButton,
    kVXObjectTypeSlider,
    kVXObjectTypeCheckBox,
    kVXObjectTypeControlSwitch,
    kVXObjectTypeMenu,
    kVXObjectTypeMenuItemImage,

    // container
    kVXObjectTypeLayout,
    kVXObjectTypeScrollView,
    kVXObjectTypePageView,
    kVXObjectTypeListView,

    // 3d
    kVXObjectTypeParticleSystem3D,

    // limit value
    kVXObjectTypeLimitValue,
};

//
// enum VXParticleType
//
enum VXParticleType
{
    kVXParticleType_General,
    kVXParticleType_Fire,
    kVXParticleType_Fireworks,
    kVXParticleType_Sun,
    kVXParticleType_Galaxy,
    kVXParticleType_Flower,
    kVXParticleType_Meteor,
    kVXParticleType_Spiral,
    kVXParticleType_Explosion,
    kVXParticleType_Smoke,
    kVXParticleType_Snow,
    kVXParticleType_Rain,
    kVXParticleType_Unknow = ~0
};

struct equal_to_i
    : public std::binary_function<std::string, std::string, bool>
{    // functor for operator==
    bool operator()(const std::string& _Left, const std::string& _Right) const
    {    // apply operator== to operands
        return (stricmp(_Left.c_str(), _Right.c_str()) == 0);
    }
};

template<typename EnumType>
class VXEnumTypeInfo
{
public:
    VXEnumTypeInfo(){}
    virtual ~VXEnumTypeInfo(){}
    VXEnumTypeInfo(const EnumType& value) : realValue(value)
    {

    }
    VXEnumTypeInfo(const std::string& strValue)
    {
        realValue = toEnumVal(strValue);
    }

    static EnumType toEnumVal(const std::string& enumKey)
    {
        auto target = tableString2Enum.find(enumKey);
        
        assert(target != tableString2Enum.end());

        return target->second;
    }

    static const std::string& toStrVal(EnumType enumVal)
    {
        auto target = tableEnum2String.find(enumVal);

        assert(target != tableEnum2String.end());

        return target->second;
    }

    friend void tableInit(void);

public:
    operator EnumType() const
    {
        return realValue;
    }

    operator const std::string&(void) const
    {
        return toStrVal(realValue);
    }

    EnumType value(void) const
    {
        return realValue;
    }

    const std::string& as_string(void) const
    {
        return toStrVal(realValue);
    }

private:
    EnumType realValue;

protected:
    static void bind(const char* strVal, EnumType enumVal)
    {
        tableString2Enum.insert(std::make_pair(strVal, enumVal));
        tableEnum2String.insert(std::make_pair(enumVal, strVal));
    }
protected:
    static std::unordered_map<EnumType, std::string> tableEnum2String;
    static std::unordered_map<std::string, EnumType, std::hash<std::string>, equal_to_i> tableString2Enum;
};

#define VXIMPL_ENUM(ClassName,EnumType) \
    std::unordered_map<std::string, EnumType, std::hash<std::string>, equal_to_i> ClassName::tableString2Enum; \
    std::unordered_map<EnumType, std::string> ClassName::tableEnum2String

#define VXDEF_ENUM(ClassName,EnumType) \
class ClassName : public VXEnumTypeInfo<EnumType> \
{ \
public: \
    ClassName(){}; \
    ClassName(const EnumType& value) : VXEnumTypeInfo<EnumType>(value) {} \
    ClassName(const std::string& strValue) : VXEnumTypeInfo<EnumType>(strValue) {} \
}

VXDEF_ENUM(VXEnumParticleMode, cocos2d::ParticleSystem::Mode);
VXDEF_ENUM(VXEnumParticlePositionType, cocos2d::ParticleSystem::PositionType);
VXDEF_ENUM(VXEnumGLBlendFunc, GLenum);
VXDEF_ENUM(VXEnumObjectType, VXObjectType);
VXDEF_ENUM(VXEnumParticleType, VXParticleType);
VXDEF_ENUM(VXEnumLocateType, VXLocateType);
VXDEF_ENUM(VXEnumLabelEffect, cocos2d::LabelEffect);
VXDEF_ENUM(VXEnumLabelTextVAlignment, cocos2d::TextVAlignment);
VXDEF_ENUM(VXEnumLabelTextHAlignment, cocos2d::TextHAlignment);
VXDEF_ENUM(VXEnumLabelFontType, cocos2d::ui::Text::Type);
VXDEF_ENUM(VXEnumLayoutType, cocos2d::ui::Layout::Type);
VXDEF_ENUM(VXEnumLayoutClippingType, cocos2d::ui::Layout::ClippingType);
VXDEF_ENUM(VXEnumLayoutBackGroundColorType, cocos2d::ui::Layout::BackGroundColorType);

VXDEF_ENUM(VXEnumScrollDirection, cocos2d::ui::ScrollView::Direction);
VXDEF_ENUM(VXEnumScrollGravityMode, cocos2d::ui::ListView::Gravity);

#endif /* _VXENUMTYPEINFO_H_ */
