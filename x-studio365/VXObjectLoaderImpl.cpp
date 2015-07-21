//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#include "VXObjectLoaderImpl.h"
#include "VXObjectLoader.h"
#include "VXObjectFactory.h"
#include "VXObjectAttribsLoader.h"
#include "VXObjectFuncTable.h"
#include "purelib/NXGameDefs.h"

#include "purelib/utils/xxfsutility.h"

using namespace vx365;

extern VXFuncTableItem internalFuncTab[];

/// ## end of object create helpers
Node*  ObjectLoaderImpl::convertFromRecursively(const xml4w::document& doc, ObjectLoader* context)
{
    auto root = doc.root();

    context->currentkey_ = "/" + root.get_name();

    VXEnumObjectType objType(root.get_name());

    // VXEnumParticleType particleType(root.get_attribute_value("particleType", "Unknow"));
    auto zobj = createObjectInternal(root, objType, context);
    // zobj->convertFrom(root);
    /*if (parent != nullptr)
        zobj->setDeep(parent->getDeep() + 1);*/
    //context->parentkey_ = "/";

    convertFromRecursively(root, zobj, context, 1);

    return zobj;
}

void ObjectLoaderImpl::convertFromRecursively(const xml4w::element& parentInfo, Node* parentNode, ObjectLoader* context, int deep)
{
    auto parentkey = context->currentkey_ + "/";
    parentInfo.cforeach([&parentkey, parentNode, context, deep](const xml4w::element& nodeInfo){
        // current child
        VXEnumObjectType objType(nodeInfo.get_name());
        auto nameAttrib = nodeInfo.get_attribute_value("name", "");
        context->currentkey_ = parentkey + (nameAttrib.empty() ? nodeInfo.get_name() : nameAttrib);
        auto newObj = createObjectInternal(nodeInfo, parentNode, objType, context);

        // configuration common attributes
        newObj->ignoreAnchorPointForPosition(false);

        ObjectAttribsLoader::loadObjectAttribs(nodeInfo, objType, newObj, context);

        if (newObj != nullptr)
        { // µÝ½ø
            convertFromRecursively(nodeInfo, newObj, context, deep + 1);
        }
    });
}

Node* ObjectLoaderImpl::createObjectInternal(const xml4w::element& levelInfo, cocos2d::Node* parentNode, VXObjectType type, ObjectLoader* context)
{
    auto newObj = createObjectInternal(levelInfo, type, context);

    auto parent = parentNode;

    if (newObj)
    {
        parentNode->addChild(newObj/*, deep*/);
    }

    return newObj;
}

Node* ObjectLoaderImpl::createObjectInternal(const xml4w::element& levelInfo, VXObjectType type, ObjectLoader* context)
{
    Node* newNode = vx365::ObjectFactory::internalCreateObject(levelInfo, type, context);

    /// init variables
    auto varb = context->variable_tab_.find(context->currentkey_);
    if (varb != context->variable_tab_.end() && varb->second != nullptr)
    {
        *varb->second = newNode;
    }

    /// register event to cocos2d-x event dispatcher TODO: impl
    /*auto wrapper_set = context->ehm_.get_handler_set(context->currentkey_);
    for (auto& wrapper : *wrapper_set)
    {
    switch (wrapper.handler->get_event_type())
    {
    case kEventTypeTouchDown:
    if (1){
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&wrapper,newNode](Touch*, Event*){
    return wrapper.handler->invoke(newNode, kEventTypeTouchDown);
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, newNode);
    }
    break;
    }
    }*/

    return newNode;
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

static Node* internalCreateSprite(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto textureKey = info.get_attribute_value("texturePath", "");
    return internalCreateSpriteImpl<Sprite>(textureKey, ctx);
}

static Node* internalCreateScale9Sprite(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto textureKey = info.get_attribute_value("texturePath", "");
    return internalCreateSpriteImpl<ui::Scale9Sprite>(textureKey, ctx);
}

static Node* internalCreateSliderBar(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto backgroundImage = info.get_attribute_value("backgroundImage", "");
    auto progressImage = info.get_attribute_value("progressImage", "");
    auto thumbImage = info.get_attribute_value("thumbImage", "");
    auto thumbSelectedImage = info.get_attribute_value("thumbSelectedImage", "");

    return extension::ControlSlider::create(
        internalCreateSpriteImpl<Sprite>(backgroundImage, ctx),
        internalCreateSpriteImpl<Sprite>(progressImage, ctx),
        internalCreateSpriteImpl<Sprite>(thumbImage, ctx),
        internalCreateSpriteImpl<Sprite>(thumbSelectedImage, ctx)
        );
}

static Node* internalCreateProgressBar(const xmldrv::element& from, ObjectLoader* ctx)
{
    auto backImage = from.get_attribute_value("backgroundImage", "");
    auto progImage = from.get_attribute_value("progressImage", "");
    return purelib::ProgressBar::create(internalCreateSpriteImpl<Sprite>(backImage, ctx),
        internalCreateSpriteImpl<Sprite>(progImage, ctx));
}

static Node* internalCreateTextField(const xmldrv::element& info, ObjectLoader* ctx)
{
    return InputBox::create(
        info.get_attribute_value("placeholder", "Default Placeholder"),
        info.get_attribute_value("fontName", "Courier New"),
        info.get_attribute_value("fontSize", 24),
        5.0f,
        info.get_attribute_value("cursorColor", Color4B::WHITE));
}

static Node* internalCreateSwitch(const xmldrv::element& info, ObjectLoader* ctx)
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

static Node* internalCreateMenuItemImage(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto normalImage = info.get_attribute_value("normalImage", "");
    auto selectedImage = info.get_attribute_value("selectedImage", "");
    auto disabledImage = info.get_attribute_value("disabledImage", "");

    // enduser can use MenuItem::setCallback
    return MenuItemSprite::create(internalCreateSpriteImpl<Sprite>(normalImage, ctx),
        internalCreateSpriteImpl<Sprite>(selectedImage, ctx),
        internalCreateSpriteImpl<Sprite>(disabledImage, ctx));
}

static Node* internalCreateLabel(const xmldrv::element& info, ObjectLoader* /*ctx*/)
{
    auto textContent = info.get_attribute_value("text", "aLabel");
    auto fontName = info.get_attribute_value("fontName", "Courier New");
    auto fontSize = info.get_attribute_value("fontSize", 24.f);
    return ui::Text::create(textContent, fontName, fontSize);
}

static Node* internalCreateLabelAtlas(const xmldrv::element& info, ObjectLoader* /*ctx*/)
{ // For Atlas, Never Use Merged Texture
    auto text = info.get_attribute_value("fontName", "Courier New");
    auto charMapFile = info.get_attribute_value("charMapFile", "");
    auto itemWidth = info.get_attribute_value("itemWidth", 17);
    auto itemHeight = info.get_attribute_value("itemHeight", 22);
    auto startCharMap = info.get_attribute_value("startCharMap", "0");
    return ui::TextAtlas::create(text, charMapFile, itemWidth, itemHeight, startCharMap);
}

static Node* internalCreateLabelBMFont(const xmldrv::element& info, ObjectLoader* /*ctx*/)
{
    auto fntFile = info.get_attribute_value("BMFontFile", "");
    auto text = info.get_attribute_value("text", "aBMFont");

    return ui::TextBMFont::create(text, fntFile);
}

static Node* internalCreateButton(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto normalImage = info.get_attribute_value("backgroundNormalImage", "");
    auto selectedImage = info.get_attribute_value("backgroundSelectedImage", "");
    auto disableImage = info.get_attribute_value("backgroundDisableImage", "");

    auto texType = !ctx->isMergedTexUsed() ? ui::TextureResType::LOCAL : ui::TextureResType::PLIST;

    return ui::Button::create(normalImage, selectedImage, disableImage, texType);
}

static Node* internalCreateCheckBox(const xmldrv::element& info, ObjectLoader* ctx)
{
    auto backgroundImage = info.get_attribute_value("backgroundImage", "");
    auto backgroundSelectedImage = info.get_attribute_value("backgroundSelectedImage", "");
    auto backgroundDisabledImage = info.get_attribute_value("backgroundDisabledImage", "");
    auto checkedImage = info.get_attribute_value("checkedImage", "");
    auto checkedDisabledImage = info.get_attribute_value("checkedDisabledImage", "");

    auto texType = !ctx->isMergedTexUsed() ? ui::TextureResType::LOCAL : ui::TextureResType::PLIST;

    return ui::CheckBox::create(backgroundImage, backgroundSelectedImage, checkedImage, backgroundDisabledImage, checkedDisabledImage, texType);
}

static Node* internalCreateParticleSystem(const xmldrv::element& info, ObjectLoader* ctx)
{
    std::string plist = info.get_attribute_value("particleSrc", "");
    if (fsutil::exists(plist.c_str()))
    {
        // this->importFile(std::move(plist));
        return cocos2d::ParticleSystemQuad::create(plist);
    }

    return cocos2d::ParticleSystemQuad::create(); // A Empty ParticleSystem
}
