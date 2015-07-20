//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#include "VXObjectLoaderImpl.h"
#include "VXObjectLoader.h"
#include "VXObjectFactory.h"
#include "VXObjectFuncTable.h"

#include "purelib/NXGameDefs.h"
#include "purelib/utils/xxfsutility.h"

using namespace vx365;

extern VXFuncTableItem internalFuncTab[];

Node* vx365::ObjectFactory::internalCreateObject(const xmldrv::element& info, VXObjectType type, ObjectLoader* context)
{
    Node* newNode = NULL;

    if (type > kVXObjectTypeNone && type < kVXObjectTypeLimitValue) {
        auto& item = internalFuncTab[type];
        if (item.lpfnCreateObject != nullptr) {
            newNode = item.lpfnCreateObject(info, context);
        }
        else {
            cocos2d::log("vx365::ObjectLoaderImpl::createObjectInternal, unsupport type: %d", type);
            assert(false);
            return nullptr;
        }
    }

    return newNode;
}

/// internal agent create functions
Node* vx365::ObjectFactory::internalCreateScene(const xmldrv::element&, ObjectLoader*)
{
    return cocos2d::Scene::create();
}

Node* vx365::ObjectFactory::internalCreateNode(const xmldrv::element&, ObjectLoader*)
{
    return cocos2d::Node::create();
}

Node* vx365::ObjectFactory::internalCreateLayerColor(const xmldrv::element&, ObjectLoader*)
{
    return cocos2d::LayerColor::create();
}

Node* vx365::ObjectFactory::internalCreateMenu(const xmldrv::element&, ObjectLoader*)
{
    return cocos2d::Menu::create();
}

Node* vx365::ObjectFactory::internalCreateScrollView(const xmldrv::element&, ObjectLoader*)
{
    return cocos2d::ui::ScrollView::create();
}

Node* vx365::ObjectFactory::internalCreateListView(const xmldrv::element&, ObjectLoader*)
{
    return cocos2d::ui::ListView::create();
}

Node* vx365::ObjectFactory::internalCreatePageView(const xmldrv::element&, ObjectLoader*)
{
    return cocos2d::ui::PageView::create();
}

Node* vx365::ObjectFactory::internalCreateLayout(const xmldrv::element& from, ObjectLoader* ctx)
{
    return cocos2d::ui::Layout::create();
}

/// internal create Node Methods impl, TODO: use merged texture cocos2d::SpriteFrameCache

template<typename _SPRITE>
static _SPRITE* internalCreateSpriteImpl(const std::string& textureKey, ObjectLoader* ctx)
{
    if (!ctx->isMergedTexUsed()) {
        return _SPRITE::create(textureKey);
    }
    else {
        return _SPRITE::createWithSpriteFrameName(textureKey);
    }
}

Node* vx365::ObjectFactory::internalCreateSprite(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto textureKey = info.get_attribute_value("texturePath", "");
    return internalCreateSpriteImpl<Sprite>(textureKey, ctx);
}

Node* vx365::ObjectFactory::internalCreateScale9Sprite(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto textureKey = info.get_attribute_value("texturePath", "");
    return internalCreateSpriteImpl<ui::Scale9Sprite>(textureKey, ctx);
}

Node* vx365::ObjectFactory::internalCreateSliderBar(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto backgroundImage = info.get_attribute_value("backgroundImage", "");
    auto progressImage = info.get_attribute_value("progressImage", "");
    auto thumbImage = info.get_attribute_value("thumbImage", "");
    auto thumbPressedImage = info.get_attribute_value("thumbPressedImage", "");
    auto thumbDisabledImage = info.get_attribute_value("thumbDisabledImage", "");

    auto resType = ctx->isMergedTexUsed() ? ui::Widget::TextureResType::PLIST : ui::Widget::TextureResType::LOCAL;

    ui::Slider* slider = ui::Slider::create();
    slider->loadBarTexture(backgroundImage, resType);
    slider->loadProgressBarTexture(progressImage, resType);
    slider->loadSlidBallTextureNormal(thumbImage, resType);
    slider->loadSlidBallTexturePressed(thumbPressedImage, resType);
    slider->loadSlidBallTextureDisabled(thumbDisabledImage, resType);
    
    return slider;
}

Node* vx365::ObjectFactory::internalCreateProgressBar(const xmldrv::element& from, ObjectLoader* ctx)
{
    auto backImage = from.get_attribute_value("backgroundImage", "");
    auto progImage = from.get_attribute_value("progressImage", "");
    return purelib::ProgressBar::create(internalCreateSpriteImpl<Sprite>(backImage, ctx),
        internalCreateSpriteImpl<Sprite>(progImage, ctx));
}

Node* vx365::ObjectFactory::internalCreateTextField(const xmldrv::element& info, ObjectLoader* ctx)
{
    return InputBox::create(
        info.get_attribute_value("placeholder", "Default Placeholder"), 
        info.get_attribute_value("fontName", "Courier New"), 
        info.get_attribute_value("fontSize", 24), 
        5.0f,
        info.get_attribute_value("cursorColor", Color4B::WHITE));
}

Node* vx365::ObjectFactory::internalCreateSwitch(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto maskImage = info.get_attribute_value("maskImage", "");
    auto onImage = info.get_attribute_value("onImage", "");
    auto offImage = info.get_attribute_value("offImage", "");
    auto thumbImage = info.get_attribute_value("thumbImage", "");

    return extension::ControlSwitch::create(
        internalCreateSpriteImpl<Sprite>(maskImage, ctx),
        internalCreateSpriteImpl<Sprite>(onImage, ctx),
        internalCreateSpriteImpl<Sprite>(offImage, ctx),
        internalCreateSpriteImpl<Sprite>(thumbImage, ctx)
        );
}

Node* vx365::ObjectFactory::internalCreateMenuItemImage(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto normalImage = info.get_attribute_value("normalImage", "");
    auto selectedImage = info.get_attribute_value("selectedImage", "");
    auto disabledImage = info.get_attribute_value("disabledImage", "");

    // enduser can use MenuItem::setCallback
    return MenuItemSprite::create(internalCreateSpriteImpl<Sprite>(normalImage, ctx),
        internalCreateSpriteImpl<Sprite>(selectedImage, ctx),
        internalCreateSpriteImpl<Sprite>(disabledImage, ctx));
}

Node* vx365::ObjectFactory::internalCreateLabel(const xmldrv::element& info, ObjectLoader* /*ctx*/)
{
    auto textContent = info.get_attribute_value("text", "aLabel");
    auto fontName = info.get_attribute_value("fontName", "Courier New");
    auto fontSize = info.get_attribute_value("fontSize", 24.f);
    return ui::Text::create(textContent, fontName, fontSize);
}

Node* vx365::ObjectFactory::internalCreateLabelAtlas(const xmldrv::element& info, ObjectLoader* /*ctx*/)
{ // For Atlas, Never Use Merged Texture
    auto text = info.get_attribute_value("fontName", "Courier New");
    auto charMapFile = info.get_attribute_value("charMapFile", "");
    auto itemWidth = info.get_attribute_value("itemWidth", 17);
    auto itemHeight = info.get_attribute_value("itemHeight", 22);
    auto startCharMap = info.get_attribute_value("startCharMap", "0");
    return ui::TextAtlas::create(text, charMapFile, itemWidth, itemHeight, startCharMap);
}

Node* vx365::ObjectFactory::internalCreateLabelBMFont(const xmldrv::element& info, ObjectLoader* /*ctx*/)
{
    auto fntFile = info.get_attribute_value("BMFontFile", "");
    auto text = info.get_attribute_value("text", "aBMFont");

    return ui::TextBMFont::create(text, fntFile);
}

Node* vx365::ObjectFactory::internalCreateButton(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto normalImage = info.get_attribute_value("backgroundNormalImage", "");
    auto selectedImage = info.get_attribute_value("backgroundSelectedImage", "");
    auto disableImage = info.get_attribute_value("backgroundDisableImage", "");

    auto texType = !ctx->isMergedTexUsed() ? ui::TextureResType::LOCAL : ui::TextureResType::PLIST;
    
    return ui::Button::create(normalImage, selectedImage, disableImage, texType);
}

Node* vx365::ObjectFactory::internalCreateCheckBox(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto backgroundImage = info.get_attribute_value("backgroundImage", "");
    auto backgroundSelectedImage = info.get_attribute_value("backgroundSelectedImage", "");
    auto backgroundDisabledImage = info.get_attribute_value("backgroundDisabledImage", "");
    auto checkedImage = info.get_attribute_value("checkedImage", "");
    auto checkedDisabledImage = info.get_attribute_value("checkedDisabledImage", "");

    auto texType = !ctx->isMergedTexUsed() ? ui::TextureResType::LOCAL : ui::TextureResType::PLIST;

    return ui::CheckBox::create(backgroundImage, backgroundSelectedImage, checkedImage, backgroundDisabledImage, checkedDisabledImage, texType);
}

Node* vx365::ObjectFactory::internalCreateParticleSystem(const xmldrv::element& info, ObjectLoader* ctx)
{
    std::string plist = info.get_attribute_value("particleSrc", "");
    if (fsutil::exists(plist.c_str()))
    {
        // this->importFile(std::move(plist));
        return cocos2d::ParticleSystemQuad::create(plist);
    }

    return cocos2d::ParticleSystemQuad::create(); // A Empty ParticleSystem
}

