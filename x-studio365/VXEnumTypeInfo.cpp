//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#include "VXEnumTypeInfo.h"

VXIMPL_ENUM(VXEnumParticleMode, cocos2d::ParticleSystem::Mode);
VXIMPL_ENUM(VXEnumParticlePositionType, cocos2d::ParticleSystem::PositionType);
VXIMPL_ENUM(VXEnumGLBlendFunc, GLenum);
VXIMPL_ENUM(VXEnumObjectType, VXObjectType);
VXIMPL_ENUM(VXEnumParticleType, VXParticleType);
VXIMPL_ENUM(VXEnumLocateType, VXLocateType);
VXIMPL_ENUM(VXEnumLabelEffect, cocos2d::LabelEffect);

VXIMPL_ENUM(VXEnumLabelTextHAlignment, cocos2d::TextHAlignment);
VXIMPL_ENUM(VXEnumLabelTextVAlignment, cocos2d::TextVAlignment);
VXIMPL_ENUM(VXEnumLabelFontType, cocos2d::ui::Text::Type);

VXIMPL_ENUM(VXEnumLayoutType, cocos2d::ui::Layout::Type);
VXIMPL_ENUM(VXEnumLayoutClippingType, cocos2d::ui::Layout::ClippingType);
VXIMPL_ENUM(VXEnumLayoutBackGroundColorType, cocos2d::ui::Layout::BackGroundColorType);

VXIMPL_ENUM(VXEnumScrollDirection, cocos2d::ui::ScrollView::Direction);
VXIMPL_ENUM(VXEnumScrollGravityMode, cocos2d::ui::ListView::Gravity);

void tableInit(void)
{
    VXEnumParticleMode::bind("RADIUS", cocos2d::ParticleSystem::Mode::RADIUS);
    VXEnumParticleMode::bind("GRAVITY", cocos2d::ParticleSystem::Mode::GRAVITY);

    VXEnumParticlePositionType::bind("FREE", cocos2d::ParticleSystem::PositionType::FREE);
    VXEnumParticlePositionType::bind("RELATIVE", cocos2d::ParticleSystem::PositionType::RELATIVE);
    VXEnumParticlePositionType::bind("GROUPED", cocos2d::ParticleSystem::PositionType::GROUPED);

    VXEnumGLBlendFunc::bind("GL_ZERO", GL_ZERO);
    VXEnumGLBlendFunc::bind("GL_ONE", GL_ONE);
    VXEnumGLBlendFunc::bind("GL_SRC_COLOR", GL_SRC_COLOR);
    VXEnumGLBlendFunc::bind("GL_ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR);
    VXEnumGLBlendFunc::bind("GL_SRC_ALPHA", GL_SRC_ALPHA);
    VXEnumGLBlendFunc::bind("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA);
    VXEnumGLBlendFunc::bind("GL_DST_ALPHA", GL_DST_ALPHA);
    VXEnumGLBlendFunc::bind("GL_ONE_MINUS_DST_ALPHA", GL_ONE_MINUS_DST_ALPHA);
    VXEnumGLBlendFunc::bind("GL_DST_COLOR", GL_DST_COLOR);
    VXEnumGLBlendFunc::bind("GL_ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR);
    VXEnumGLBlendFunc::bind("GL_SRC_ALPHA_SATURATE", GL_SRC_ALPHA_SATURATE);

    VXEnumObjectType::bind("Node", kVXObjectTypeNode);
    VXEnumObjectType::bind("Sprite", kVXObjectTypeSprite);
    VXEnumObjectType::bind("Scale9Sprite", kVXObjectTypeScale9Sprite);

    VXEnumObjectType::bind("Layer", kVXObjectTypeLayer);
    VXEnumObjectType::bind("LayerColor", kVXObjectTypeLayerColor);

    VXEnumObjectType::bind("Button", kVXObjectTypeButton);
    VXEnumObjectType::bind("Text", kVXObjectTypeLabel);
    VXEnumObjectType::bind("InputBox", kVXObjectTypeTextField);
    VXEnumObjectType::bind("Slider", kVXObjectTypeSlider);
    VXEnumObjectType::bind("ProgressBar", kVXObjectTypeProgressBar);
    VXEnumObjectType::bind("CheckBox", kVXObjectTypeCheckBox);

    VXEnumObjectType::bind("Menu", kVXObjectTypeMenu);
    VXEnumObjectType::bind("Layout", kVXObjectTypeLayout);
    VXEnumObjectType::bind("ScrollView", kVXObjectTypeScrollView);
    VXEnumObjectType::bind("PageView", kVXObjectTypePageView);
    VXEnumObjectType::bind("ListView", kVXObjectTypeListView);
    VXEnumObjectType::bind("RichLabel", kVXObjectTypeRichLabel);
    VXEnumObjectType::bind("TextAtlas", kVXObjectTypeLabelAtlas);
    VXEnumObjectType::bind("TextBMFont", kVXObjectTypeLabelBMFont);
    VXEnumObjectType::bind("ControlSwitch", kVXObjectTypeControlSwitch);

    VXEnumObjectType::bind("ParticleSystem", kVXObjectTypeParticleSystem);
    VXEnumObjectType::bind("Scene", kVXObjectTypeScene);

    VXEnumObjectType::bind("ParticleSystem3D_x365", kVXObjectTypeParticleSystem3D);

    VXEnumParticleType::bind("General", kVXParticleType_General);
    VXEnumParticleType::bind("Fire", kVXParticleType_Fire);
    VXEnumParticleType::bind("Fireworks", kVXParticleType_Fireworks);
    VXEnumParticleType::bind("Sun", kVXParticleType_Sun);
    VXEnumParticleType::bind("Galaxy", kVXParticleType_Galaxy);
    VXEnumParticleType::bind("Flower", kVXParticleType_Flower);
    VXEnumParticleType::bind("Meteor", kVXParticleType_Meteor);
    VXEnumParticleType::bind("Spiral", kVXParticleType_Spiral);
    VXEnumParticleType::bind("Explosion", kVXParticleType_Explosion);
    VXEnumParticleType::bind("Smoke", kVXParticleType_Smoke);
    VXEnumParticleType::bind("Snow", kVXParticleType_Snow);
    VXEnumParticleType::bind("Rain", kVXParticleType_Rain);
    VXEnumParticleType::bind("Unknow", kVXParticleType_Unknow);

    VXEnumLocateType::bind("FREE", VXLocateType::LOCATE_FREE);
    VXEnumLocateType::bind("LEFT_TOP", VXLocateType::LOCATE_LEFT_TOP);
    VXEnumLocateType::bind("LEFT_BOTTOM", VXLocateType::LOCATE_LEFT_BOTTOM);
    VXEnumLocateType::bind("RIGHT_TOP", VXLocateType::LOCATE_RIGHT_TOP);
    VXEnumLocateType::bind("RIGHT_BOTTOM", VXLocateType::LOCATE_RIGHT_BOTTOM);
    VXEnumLocateType::bind("CENTER", VXLocateType::LOCATE_CENTER);
    VXEnumLocateType::bind("CENTER_LEFT", VXLocateType::LOCATE_CENTER_LEFT);
    VXEnumLocateType::bind("CENTER_TOP", VXLocateType::LOCATE_CENTER_TOP);
    VXEnumLocateType::bind("CENTER_RIGHT", VXLocateType::LOCATE_CENTER_RIGHT);
    VXEnumLocateType::bind("CENTER_BOTTOM", VXLocateType::LOCATE_CENTER_BOTTOM);


    VXEnumLabelEffect::bind("NORMAL", cocos2d::LabelEffect::NORMAL);
    VXEnumLabelEffect::bind("OUTLINE", cocos2d::LabelEffect::OUTLINE);
    VXEnumLabelEffect::bind("SHADOW", cocos2d::LabelEffect::SHADOW);
    VXEnumLabelEffect::bind("GLOW", cocos2d::LabelEffect::GLOW);

    // TextVAlignment
    VXEnumLabelTextVAlignment::bind("TOP", cocos2d::TextVAlignment::TOP);
    VXEnumLabelTextVAlignment::bind("CENTER", cocos2d::TextVAlignment::CENTER);
    VXEnumLabelTextVAlignment::bind("BOTTOM", cocos2d::TextVAlignment::BOTTOM);

    // TextHAlignment
    VXEnumLabelTextHAlignment::bind("LEFT", cocos2d::TextHAlignment::LEFT);
    VXEnumLabelTextHAlignment::bind("CENTER", cocos2d::TextHAlignment::CENTER);
    VXEnumLabelTextHAlignment::bind("RIGHT", cocos2d::TextHAlignment::RIGHT);

    VXEnumLabelFontType::bind("SYSTEM", cocos2d::ui::Text::Type::SYSTEM);
    VXEnumLabelFontType::bind("TTF", cocos2d::ui::Text::Type::TTF);

    /// layout
    VXEnumLayoutType::bind("ABSOLUTE", cocos2d::ui::Layout::Type::ABSOLUTE);
    VXEnumLayoutType::bind("VERTICAL", cocos2d::ui::Layout::Type::VERTICAL);
    VXEnumLayoutType::bind("HORIZONTAL", cocos2d::ui::Layout::Type::HORIZONTAL);
    VXEnumLayoutType::bind("RELATIVE", cocos2d::ui::Layout::Type::RELATIVE);

    VXEnumLayoutClippingType::bind("STENCIL", cocos2d::ui::Layout::ClippingType::STENCIL);
    VXEnumLayoutClippingType::bind("SCISSOR", cocos2d::ui::Layout::ClippingType::SCISSOR);

    VXEnumLayoutBackGroundColorType::bind("NONE", cocos2d::ui::Layout::BackGroundColorType::NONE);
    VXEnumLayoutBackGroundColorType::bind("SOLID", cocos2d::ui::Layout::BackGroundColorType::SOLID);
    VXEnumLayoutBackGroundColorType::bind("GRADIENT", cocos2d::ui::Layout::BackGroundColorType::GRADIENT);

    // scroll view
    VXEnumScrollDirection::bind("NONE", cocos2d::ui::ScrollView::Direction::NONE);
    VXEnumScrollDirection::bind("VERTICAL", cocos2d::ui::ScrollView::Direction::VERTICAL);
    VXEnumScrollDirection::bind("HORIZONTAL", cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    VXEnumScrollDirection::bind("BOTH", cocos2d::ui::ScrollView::Direction::BOTH);

    VXEnumScrollGravityMode::bind("LEFT", cocos2d::ui::ListView::Gravity::LEFT);
    VXEnumScrollGravityMode::bind("RIGHT", cocos2d::ui::ListView::Gravity::RIGHT);
    VXEnumScrollGravityMode::bind("CENTER_HORIZONTAL", cocos2d::ui::ListView::Gravity::CENTER_HORIZONTAL);
    VXEnumScrollGravityMode::bind("TOP", cocos2d::ui::ListView::Gravity::TOP);
    VXEnumScrollGravityMode::bind("BOTTOM", cocos2d::ui::ListView::Gravity::BOTTOM);
    VXEnumScrollGravityMode::bind("CENTER_VERTICAL", cocos2d::ui::ListView::Gravity::CENTER_VERTICAL);
}

const int INIT_ENUM_TABS = (tableInit(), 0);
