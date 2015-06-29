//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#include "VXObjectLoaderImpl.h"
#include "VXObjectLoader.h"
#include "VXObjectAttribsLoader.h"
#include "VXObjectFactory.h"
#include "purelib/NXGameDefs.h"
#include "VXObjectFuncTable.h"
#include "purelib/NXGameDefs.h"
#include "purelib/utils/xxfsutility.h"
#include <utility>

using namespace vx365;


VXFuncTableItem internalFuncTab[] = {
    { kVXObjectTypeNone, nullptr, nullptr },
    { kVXObjectTypeScene, &vx365::ObjectFactory::internalCreateScene, nullptr },
    { kVXObjectTypeNode, &vx365::ObjectFactory::internalCreateNode, &ObjectAttribsLoader::loadAttribsOfNode },
    { kVXObjectTypeSprite, &vx365::ObjectFactory::internalCreateSprite, &ObjectAttribsLoader::loadAttribsOfSprite },
    { kVXObjectTypeLayer, nullptr, nullptr },
    { kVXObjectTypeLayerColor, &vx365::ObjectFactory::internalCreateLayerColor, &ObjectAttribsLoader::loadAttribsOfNode },
    { kVXObjectTypeScale9Sprite, &vx365::ObjectFactory::internalCreateScale9Sprite, &ObjectAttribsLoader::loadAttribsOfScale9Sprite },
    { kVXObjectTypeLabel, &vx365::ObjectFactory::internalCreateLabel, &ObjectAttribsLoader::loadAttribsOfLabel },
    { kVXObjectTypeProgressBar, &vx365::ObjectFactory::internalCreateProgressBar, &ObjectAttribsLoader::loadAttribsOfProgressBar },
    { kVXObjectTypeParticleSystem, &vx365::ObjectFactory::internalCreateParticleSystem, &ObjectAttribsLoader::loadAttribsOfParticleSystem },
    { kVXObjectTypeLabelAtlas, &vx365::ObjectFactory::internalCreateLabelAtlas, &ObjectAttribsLoader::loadAttribsOfLabelAtlas },
    { kVXObjectTypeLabelBMFont, &vx365::ObjectFactory::internalCreateLabelBMFont, &ObjectAttribsLoader::loadAttribsOfLabelBMFont },
    { kVXObjectTypeRichLabel, nullptr, nullptr },
    { kVXObjectTypeTextField, &vx365::ObjectFactory::internalCreateTextField, &ObjectAttribsLoader::loadAttribsOfTextField },
    { kVXObjectTypeButton, &vx365::ObjectFactory::internalCreateButton, &ObjectAttribsLoader::loadAttribsOfButton },
    { kVXObjectTypeSlider, &vx365::ObjectFactory::internalCreateSliderBar, &ObjectAttribsLoader::loadAttribsOfSliderBar },
    { kVXObjectTypeCheckBox, &vx365::ObjectFactory::internalCreateCheckBox, &ObjectAttribsLoader::loadAttribsOfCheckBox },
    { kVXObjectTypeControlSwitch, &vx365::ObjectFactory::internalCreateSwitch, &ObjectAttribsLoader::loadAttribsOfSwitch },
    { kVXObjectTypeMenu, &vx365::ObjectFactory::internalCreateMenu, nullptr },
    { kVXObjectTypeMenuItemImage, &vx365::ObjectFactory::internalCreateMenuItemImage, nullptr },
    { kVXObjectTypeLayout, &vx365::ObjectFactory::internalCreateLayout, &ObjectAttribsLoader::loadAttribsOfLayout },
    { kVXObjectTypeScrollView, &vx365::ObjectFactory::internalCreateScrollView, &ObjectAttribsLoader::loadAttribsOfScrollView },
    { kVXObjectTypePageView, &vx365::ObjectFactory::internalCreatePageView, &ObjectAttribsLoader::loadAttribsOfPageView },
    { kVXObjectTypeListView, &vx365::ObjectFactory::internalCreateListView, &ObjectAttribsLoader::loadAttribsOfListView },
    { kVXObjectTypeParticleSystem3D, nullptr, nullptr },
    { kVXObjectTypeLimitValue, nullptr, nullptr },
};

static const size_t s_func_table_size = ARRAY_SIZE(internalFuncTab);
