//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#include "VXObjectLoaderImpl.h"
#include "VXObjectLoader.h"
#include "VXObjectAttribsLoader.h"

#include "purelib/NXGameDefs.h"

using namespace x365;

/// microsoft window tagRECT
struct MSWRect
{
	MSWRect(void) : left(0), top(0), right(0), bottom(0){}
	MSWRect(float l, float t, float r, float b) : left(l), top(t), right(r), bottom(b){}
	void  setLeft(float l){ left = l; }
	void  setTop(float t) { top = t; }
	void  setRight(float r) { right = r; }
	void  setBottom(float b) { bottom = b; }
	float getLeft(void) const{ return left; }
	float getTop(void) const { return top; }
	float getRight(void) const { return right; }
	float getBottom(void) const { return bottom; }
	float left;
	float top;
	float right;
	float bottom;
};

template<typename _SetFunc>
static void readVec2(const char* key, const xml4w::element& valueS, const _SetFunc& setValue)
{
	std::string rawVal = valueS.get_attribute_value(key, "1,1");
	auto value2f = nsconv::parse2f(rawVal.c_str(), ',');

	cocos2d::Vec2 value(std::get<0>(value2f), std::get<1>(value2f));
	setValue(value);
}

template<typename _SetFunc>
static void readColor3B(const char* key, const xml4w::element& valueS, const _SetFunc& setValue)
{
	std::string rawVal = valueS.get_attribute_value(key, "255,255,255");
	auto value3u = nsconv::parse3i(rawVal.c_str(), ',');

	cocos2d::Color3B value((unsigned char)std::get<0>(value3u), (unsigned char)std::get<1>(value3u), (unsigned char)std::get<2>(value3u));
	setValue(value);
}

template<typename _SetFunc>
static void readMSWRect(const char* key, const xml4w::element& valueS, const _SetFunc& setValue)
{
	std::string rawVal = valueS.get_attribute_value(key, "0.0,0.0,0.0,0.0");
	auto f4 = nsconv::parse4f(rawVal.c_str(), ',');
	setValue({ std::get<0>(f4), std::get<1>(f4), std::get<2>(f4), std::get<3>(f4) });
}

//static void writeCCRect(const char* key, const cocos2d::Rect& value, xml4w::element& valueS)
//{
//    char svalue[128] = { 0 };
//    sprintf(svalue, "%.3f,%.3f,%.3f,%.3f", value.origin.x, value.origin.y, value.size.width, value.size.height);
//    valueS.set_attribute_value(key, svalue);
//}

template<typename _SetFunc>
static void readCCRect(const char* key, const xml4w::element& valueS, const _SetFunc& setValue)
{
	std::string rawVal = valueS.get_attribute_value(key, "0.0,0.0,0.0,0.0");
	auto f4 = nsconv::parse4f(rawVal.c_str(), ',');
	setValue({ std::get<0>(f4), std::get<1>(f4), std::get<2>(f4), std::get<3>(f4) });
}

static void setNodeMargin(Node* detail, VXLocateType locateType, const MSWRect& margin)
{

	switch (locateType)
		{
		case VXLocateType::LOCATE_LEFT_TOP:
			nodes_layout::vscr::setNodeLT(detail, cocos2d::Vec2(margin.getLeft(), margin.getTop()));
			break;
		case VXLocateType::LOCATE_LEFT_BOTTOM:
			nodes_layout::vscr::setNodeLB(detail, cocos2d::Vec2(margin.getLeft(), margin.getBottom()));
			break;
		case VXLocateType::LOCATE_RIGHT_TOP:
			nodes_layout::vscr::setNodeRT(detail, cocos2d::Vec2(margin.getRight(), margin.getTop()));
			break;
		case VXLocateType::LOCATE_RIGHT_BOTTOM:
			nodes_layout::vscr::setNodeRB(detail, cocos2d::Vec2(margin.getRight(), margin.getBottom()));
			break;
		case VXLocateType::LOCATE_CENTER:
			nodes_layout::vscr::centerNode(detail);
			break;
		case VXLocateType::LOCATE_CENTER_LEFT:
			nodes_layout::vscr::setNodeLeft(detail, margin.getLeft());
			nodes_layout::vscr::centerNodeY(detail);
			break;
		case VXLocateType::LOCATE_CENTER_TOP:
			nodes_layout::vscr::setNodeTop(detail, margin.getTop());
			nodes_layout::vscr::centerNodeX(detail);
			break;
		case VXLocateType::LOCATE_CENTER_RIGHT:
			nodes_layout::vscr::setNodeRight(detail, margin.getRight());
			nodes_layout::vscr::centerNodeY(detail);
			break;
		case VXLocateType::LOCATE_CENTER_BOTTOM:
			nodes_layout::vscr::setNodeBottom(detail, margin.getBottom());
			nodes_layout::vscr::centerNodeX(detail);
			break;
		default:; // do nothing
		}
}

void ObjectAttribsLoader::loadObjectAttribs(const xmldrv::element& s, VXObjectType objType, Node* n)
{
	// TODO: use unordered_map
	switch (objType)
	{
	case kVXObjectTypeScene:
		break;
	case kVXObjectTypeNode:
		loadAttribsOfNode(s, n);
		break;
	case kVXObjectTypeSprite:
		loadAttribsOfSprite(s, n);
		break;
	case kVXObjectTypeScale9Sprite:
		loadAttribsOfScale9Sprite(s, n);
		break;
	case kVXObjectTypeLayerColor:
		loadAttribsOfNode(s, n);
		break;
		//   case kVXObjectTypeMenu: // TODO: support
		//       newNode = Menu::create();
		//       break;
		//case kVXObjectTypeMenuItemImage: // TODO: support
		//       newNode = MenuItemImage::create();
		//       break;
	case kVXObjectTypeLabel:
		loadAttribsOfLabel(s, n);
		break;
	case kVXObjectTypeButton:
		loadAttribsOfButton(s, n);
		break;
	case kVXObjectTypeParticleSystem:
		loadAttribsOfParticleSystem(s, n);
		break;
	case kVXObjectTypeSlider:
		loadAttribsOfSliderBar(s, n);
		break;
	case kVXObjectTypeProgressBar:
		loadAttribsOfProgressBar(s, n);
		break;
	case kVXObjectTypeCheckBox:
		loadAttribsOfCheckBox(s, n);
		break;
	case kVXObjectTypeTextField:
		loadAttribsOfTextField(s, n);
		break;
	case kVXObjectTypeScrollView:
		loadAttribsOfScrollView(s, n);
		break;
	case kVXObjectTypePageView:
		loadAttribsOfPageView(s, n);
		break;
	case kVXObjectTypeListView:
		loadAttribsOfListView(s, n);
		break;
	case kVXObjectTypeLabelAtlas:
		loadAttribsOfLabelAtlas(s, n);
		break;
	case kVXObjectTypeLabelBMFont:
		loadAttribsOfLabelBMFont(s, n);
		break;
		/*case kVXObjectTypeRichLabel: has bug, unsupport yet.
		newNode = ui::RichText::create();
		break;*/
	case kVXObjectTypeControlSwitch:
		loadAttribsOfSwitch(s, n);
		break;
	case kVXObjectTypeLayout:
		loadAttribsOfLayout(s, n);
		break;
	default:
		assert(false);
		return;
	}
}

void ObjectAttribsLoader::loadAttribsOfNode(const xmldrv::element& valueS, Node* n)
{
	auto self = n;
	assert(self);

	// this->setCustomized(valueS.get_attribute_value("customized", false));
	auto useNormalPos = valueS.get_attribute_value("normalizedPositionUsed", false);

	self->setName(valueS.get_attribute_value("name", ""));
	self->setRotation(valueS.get_attribute_value("rotation", 0.0f));
	self->setLocalZOrder(valueS.get_attribute_value("localZOrder", 0));
	self->setVertexZ(valueS.get_attribute_value("vertexZ", 0));
	self->setTag(valueS.get_attribute_value("tag", -1));
	self->setOpacity(valueS.get_attribute_value("opacity", 255));
	self->setVisible(valueS.get_attribute_value("visible", true));

	readVec2("scale", valueS, [self](const cocos2d::Vec2& value){ self->setScale2D(value); });
	
	if (useNormalPos)
		readVec2("normalizedPosition", valueS, [self](const cocos2d::Vec2& value){ self->setNormalizedPosition(value); });
	
	readVec2("anchorPoint", valueS, [self](const cocos2d::Vec2& value){ self->setAnchorPoint(value); });
	readVec2("position", valueS, [self](const cocos2d::Vec2& value){ self->setPosition(value); });
	readVec2("size", valueS, [self](const cocos2d::Vec2& value){ self->setContentSize(cocos2d::Size(value.x, value.y)); });
	readColor3B("color", valueS, [self](const cocos2d::Color3B& value){ self->setColor(value); });

	auto locateType = VXEnumLocateType::toEnumVal(valueS.get_attribute_value("locateType", "LOCATE_FREE"));
	readMSWRect("margin", valueS, [self, locateType](const MSWRect& value){
		setNodeMargin(self, locateType, value);
	});
}

void ObjectAttribsLoader::loadAttribsOfSprite(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto gameobj = dynamic_cast<cocos2d::Sprite*>(n);

	gameobj->setTexture(from.get_attribute_value("texturePath", "")); // TODO: use default image
	gameobj->setFlippedX(from.get_attribute_value("flippedX", false));
	gameobj->setFlippedY(from.get_attribute_value("flippedY", false));
	readCCRect("textureRect", from, [gameobj](const cocos2d::Rect& value){ gameobj->setTextureRect(value); });
}

static void setScale9TexturePath(ui::Scale9Sprite* detail, const std::string& path)
{
	auto texture = TextureCache::getInstance()->addImage(path);
	if (texture != nullptr)
	{
		auto sprite = Sprite::create(path);

		detail->updateWithSprite(sprite, Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height), false, Rect::ZERO);
	}
}

void ObjectAttribsLoader::loadAttribsOfScale9Sprite(const xmldrv::element& from, Node* n )
{
	loadAttribsOfNode(from, n);

	auto gameobj = dynamic_cast<cocos2d::ui::Scale9Sprite*>(n);

	setScale9TexturePath(gameobj, from.get_attribute_value("texturePath", "")); // TODO: init when create
	gameobj->setFlippedX(from.get_attribute_value("flippedX", false));
	gameobj->setFlippedY(from.get_attribute_value("flippedY", false));
	// readCCRect("textureRect", from, [gameobj](const cocos2d::Rect& value){ gameobj->setTextureRect(value); }); maybe need init when create
	readCCRect("capInsetsRect", from, [gameobj](const cocos2d::Rect& value){ gameobj->setCapInsets(value); });
}

void ObjectAttribsLoader::loadAttribsOfLabel(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto label = dynamic_cast<cocos2d::ui::Text*>(n);
	label->setString(from.get_attribute_value("text", ""));
	label->setTextColor(from.get_attribute_value("textColor", label->getTextColor()));
	label->setFontName(from.get_attribute_value("fontName", label->getFontName()));
	label->setFontSize(from.get_attribute_value("fontSize", label->getFontSize()));
	label->setTextVerticalAlignment(VXEnumLabelTextVAlignment::toEnumVal(from.get_attribute_value("textVerticalAlignment", VXEnumLabelTextVAlignment::toStrVal(label->getTextVerticalAlignment()))));
	label->setTextHorizontalAlignment(VXEnumLabelTextHAlignment::toEnumVal(from.get_attribute_value("textHorizontalAlignment", VXEnumLabelTextHAlignment::toStrVal(label->getTextHorizontalAlignment()))));
	auto effectType = VXEnumLabelEffect::toEnumVal(from.get_attribute_value("effectType", VXEnumLabelEffect::toStrVal(cocos2d::LabelEffect::NORMAL)));

	switch (effectType)
	{
	case LabelEffect::OUTLINE:
		label->enableOutline(from.get_attribute_value("outlineColor", cocos2d::Color4B::WHITE),
			from.get_attribute_value("outlineSize", 1));
		break;
	case LabelEffect::GLOW:
		label->enableGlow(from.get_attribute_value("glowColor", cocos2d::Color4B::WHITE));
		break;
	case LabelEffect::SHADOW:
		label->enableShadow(from.get_attribute_value("shadowColor", cocos2d::Color4B::WHITE),
			from.get_attribute_value("shadowOffset", cocos2d::Size(-2, -2)),
			from.get_attribute_value("shadowBlurRadius", 0));
		break;
	}
}

void ObjectAttribsLoader::loadAttribsOfLabelAtlas(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto atlas = dynamic_cast<cocos2d::ui::TextAtlas*>(n);
	auto value = from.get_attribute_value("text", "");
	auto charMapFile = from.get_attribute_value("charMapFile", "");
	auto itemWidth = from.get_attribute_value("itemWidth", 0);
	auto itemHeight = from.get_attribute_value("itemHeight", 0);
	auto startCharMap = from.get_attribute_value("startCharMap", "");

	atlas->setProperty(value, charMapFile, itemWidth, itemHeight, startCharMap);
}

void ObjectAttribsLoader::loadAttribsOfLabelBMFont(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto labelbmf = dynamic_cast<cocos2d::ui::TextBMFont*>(n);
	labelbmf->setFntFile(from.get_attribute_value("BMFontFile", ""));
	labelbmf->setString(from.get_attribute_value("text", ""));
}

void ObjectAttribsLoader::loadAttribsOfParticleSystem(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);
}

void ObjectAttribsLoader::loadAttribsOfTextField(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto detail = dynamic_cast<purelib::InputBox*>(n);
	detail->setTextColor(from.get_attribute_value("textColor", detail->getTextColor()));
	detail->setColorSpaceHolder(from.get_attribute_value("placeHolderColor", detail->getColorSpaceHolder()));
	detail->setCursorColor(from.get_attribute_value("cursorColor", detail->getCursorColor()));

	detail->setTextFontName(from.get_attribute_value("fontName", detail->getTextFontName()));
	detail->setTextFontSize(from.get_attribute_value("fontSize", detail->getTextFontSize()));

	detail->setSecureTextEntry(from.get_attribute_value("isPassword", detail->isSecureTextEntry()));
	detail->setEnabled(from.get_attribute_value("enabled", detail->isEnabled()));
	detail->setEditable(from.get_attribute_value("editable", detail->isEditable()));
	detail->setPlaceHolder(from.get_attribute_value("placeHolder", detail->getPlaceHolder()));
	detail->setString(from.get_attribute_value("text", detail->getString()));
}

void ObjectAttribsLoader::loadAttribsOfButton(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto detail = dynamic_cast<cocos2d::ui::Button*>(n);
	detail->setZoomScale(from.get_attribute_value("zoomScale", detail->getZoomScale()));
	detail->setEnabled(from.get_attribute_value("enabled", detail->isEnabled()));
	detail->setTitleText(from.get_attribute_value("titleText", detail->getTitleText()));
	detail->setTitleColor(from.get_attribute_value("titleColor", detail->getTitleColor()));
	detail->setTitleFontName(from.get_attribute_value("titleFontName", detail->getTitleFontName()));
	detail->setTitleFontSize(from.get_attribute_value("titleFontSize", detail->getTitleFontSize()));

	detail->setUnifySizeEnabled(from.get_attribute_value("unifySizeEnabled", false));
	detail->ignoreContentAdaptWithSize(from.get_attribute_value("ignoreContentAdaptWithSize", true));
	detail->loadTextureNormal(from.get_attribute_value("backgroundNormalImage", ""));
	detail->loadTexturePressed(from.get_attribute_value("backgroundSelectedImage", ""));
	detail->loadTextureDisabled(from.get_attribute_value("backgroundDisableImage", ""));

	detail->setScale9Enabled(from.get_attribute_value("scale9Enabled", detail->isScale9Enabled()));
	detail->setCapInsetsNormalRenderer(from.get_attribute_value("backgroundNormalCapInsetsRect", detail->getCapInsetsNormalRenderer()));
	detail->setCapInsetsPressedRenderer(from.get_attribute_value("backgroundSelectedCapInsetsRect", detail->getCapInsetsPressedRenderer()));
	detail->setCapInsetsDisabledRenderer(from.get_attribute_value("backgroundDisabledCapInsetsRect", detail->getCapInsetsDisabledRenderer()));
}

void ObjectAttribsLoader::loadAttribsOfSliderBar(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto detial = dynamic_cast<cocos2d::extension::ControlSlider*>(n);
	detial->setEnabled(from.get_attribute_value("enabled", true));
	detial->setMaximumValue(from.get_attribute_value("maximumValue", detial->getMaximumValue()));
	detial->setMinimumValue(from.get_attribute_value("minimumValue", detial->getMinimumValue()));
	detial->setMaximumAllowedValue(from.get_attribute_value("maximumAllowedValue", detial->getMaximumAllowedValue()));
	detial->setMinimumAllowedValue(from.get_attribute_value("minimumAllowedValue", detial->getMinimumAllowedValue()));
	detial->setValue(from.get_attribute_value("value", detial->getValue()));
}

void ObjectAttribsLoader::loadAttribsOfProgressBar(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto detail = dynamic_cast<purelib::ProgressBar*>(n);
	detail->getBackgroundSprite()->setTexture(from.get_attribute_value("backgroundImage", ""));
	detail->getProgressSprite()->setTexture(from.get_attribute_value("progressImage", ""));

	detail->updateTexture();
}

void ObjectAttribsLoader::loadAttribsOfCheckBox(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto detail = dynamic_cast<cocos2d::ui::CheckBox*>(n);

	detail->setZoomScale(from.get_attribute_value("zoomScale", detail->getZoomScale()));
	detail->setEnabled(from.get_attribute_value("enabled", detail->isEnabled()));
	detail->setSelected(from.get_attribute_value("checked", detail->isSelected()));

	detail->setUnifySizeEnabled(from.get_attribute_value("unifySizeEnabled", false));
	detail->ignoreContentAdaptWithSize(from.get_attribute_value("ignoreContentAdaptWithSize", true));

	detail->loadTextureBackGround(from.get_attribute_value("backgroundImage", ""));
	detail->loadTextureBackGroundSelected(from.get_attribute_value("backgroundSelectedImage", ""));
	detail->loadTextureBackGroundDisabled(from.get_attribute_value("backgroundDisabledImage", ""));
	detail->loadTextureFrontCross(from.get_attribute_value("checkedImage", ""));
	detail->loadTextureFrontCrossDisabled(from.get_attribute_value("checkedDisabledImage", ""));
}

void ObjectAttribsLoader::loadAttribsOfSwitch(const xmldrv::element& from, Node* n)
{
	auto detail = dynamic_cast<cocos2d::extension::ControlSwitch*>(n);

	detail->setOn(from.get_attribute_value("on", false));
}

static void  setLayoutGradientRotation(cocos2d::ui::Layout* layout, float degree)
{
	auto adjust = CC_DEGREES_TO_RADIANS(degree);
	layout->setBackGroundColorVector(cocos2d::Vec2::forAngle(adjust));
}
void ObjectAttribsLoader::loadAttribsOfLayout(const xmldrv::element& from, Node* n)
{
	loadAttribsOfNode(from, n);

	auto detail = dynamic_cast<cocos2d::ui::Layout*>(n);

	detail->setLayoutType(VXEnumLayoutType::toEnumVal(from.get_attribute_value("layoutType", VXEnumLayoutType::toStrVal(detail->getLayoutType()))));
	detail->setBackGroundColorType(VXEnumLayoutBackGroundColorType::toEnumVal(from.get_attribute_value("backgroundColorType", VXEnumLayoutBackGroundColorType::toStrVal(detail->getBackGroundColorType()))));
	detail->setBackGroundColor(from.get_attribute_value("backgroundColor", Color3B::WHITE));
	detail->setBackGroundColorOpacity(from.get_attribute_value("backgroundColorOpacity", detail->getBackGroundColorOpacity()));
	
	detail->setBackGroundColor(from.get_attribute_value("backgroundStartColor", Color3B::WHITE), from.get_attribute_value("backgroundEndColor", Color3B::BLACK));
	
	detail->setUnifySizeEnabled(from.get_attribute_value("unifySizeEnabled", false));
	detail->ignoreContentAdaptWithSize(from.get_attribute_value("ignoreContentAdaptWithSize", true));
	detail->setBackGroundImage(from.get_attribute_value("backgroundImage", ""));
	detail->setBackGroundImageColor(from.get_attribute_value("backgroundImageColor", detail->getBackGroundImageColor()));
	detail->setBackGroundImageOpacity(from.get_attribute_value("backgroundImageOpacity", (unsigned int)detail->getBackGroundImageOpacity()));

	detail->setFocused(from.get_attribute_value("focusLooped", detail->isLoopFocus()));
	detail->setPassFocusToChild(from.get_attribute_value("passFocusToChild", detail->isPassFocusToChild()));
	setLayoutGradientRotation(detail, from.get_attribute_value("gradientRotation", 0));

	detail->setClippingEnabled(from.get_attribute_value("clippingEnabled", detail->isClippingEnabled()));
	detail->setClippingType(VXEnumLayoutClippingType::toEnumVal(from.get_attribute_value("clippingType", VXEnumLayoutClippingType::toStrVal(detail->getClippingType()))));
	detail->setBackGroundImageScale9Enabled(from.get_attribute_value("backgroundImageScale9Enabled", detail->isBackGroundImageScale9Enabled()));
	detail->setBackGroundImageCapInsets(from.get_attribute_value("backgroundImageCapInsets", detail->getBackGroundImageCapInsets()));
}

void ObjectAttribsLoader::loadAttribsOfScrollView(const xmldrv::element& from, Node* n)
{
	loadAttribsOfLayout(from, n);

	auto detail = dynamic_cast<cocos2d::ui::ScrollView*>(n);
	detail->setBounceEnabled(from.get_attribute_value("bounceEnabled", detail->isBounceEnabled()));
	detail->setInertiaScrollEnabled(from.get_attribute_value("inertiaScrollEnabled", detail->isInertiaScrollEnabled()));
	detail->setInnerContainerSize(from.get_attribute_value("innerContainerSize", detail->getInnerContainerSize()));
	detail->setDirection(VXEnumScrollDirection::toEnumVal(from.get_attribute_value("direction", VXEnumScrollDirection::toStrVal(detail->getDirection()))));
}

void ObjectAttribsLoader::loadAttribsOfPageView(const xmldrv::element& from, Node* n)
{
	loadAttribsOfLayout(from, n);

	auto detail = dynamic_cast<cocos2d::ui::PageView*>(n);
	detail->setUsingCustomScrollThreshold(from.get_attribute_value("usingCustomScrollThreshold", detail->isUsingCustomScrollThreshold()));
	if (detail->isUsingCustomScrollThreshold())
		detail->setCustomScrollThreshold(from.get_attribute_value("customScrollThreshold", detail->getCustomScrollThreshold()));
}

void ObjectAttribsLoader::loadAttribsOfListView(const xmldrv::element& from, Node* n)
{
	loadAttribsOfScrollView(from, n);

	// list view spec
	auto detail = dynamic_cast<cocos2d::ui::ListView*>(n);
	detail->setItemsMargin(from.get_attribute_value("itemsMargin", detail->getItemsMargin()));
	detail->setGravity(VXEnumScrollGravityMode::toEnumVal(from.get_attribute_value("scrollGravityMode", VXEnumScrollGravityMode::toStrVal(cocos2d::ui::ListView::Gravity::CENTER_VERTICAL))));
}
