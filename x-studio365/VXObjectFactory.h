//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#ifndef _VX365_OBJECT_FACTORY_H_
#define _VX365_OBJECT_ATTRIBS_H_

#include <unordered_map>
#include <functional>
#include <cocos2d.h>
#include "VXEnumTypeInfo.h"
#include "purelib/utils/xmldrv.h"
USING_NS_CC;

namespace vx365 {

    class ObjectLoader;

    class ObjectFactory
    {
        PLIB_STATICIZE_CLASS(ObjectFactory);
    public:
        static Node* internalCreateObject(const xmldrv::element&, VXObjectType, ObjectLoader*);


        /// object create helpers
        static Node* internalCreateScene(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateNode(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateLayerColor(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateMenu(const xmldrv::element&, ObjectLoader*);

        static Node* internalCreateScrollView(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateListView(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreatePageView(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateLayout(const xmldrv::element&, ObjectLoader*);

        static Node* internalCreateSprite(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateScale9Sprite(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateSliderBar(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateProgressBar(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateTextField(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateSwitch(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateMenuItemImage(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateLabel(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateLabelAtlas(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateLabelBMFont(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateButton(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateCheckBox(const xmldrv::element&, ObjectLoader*);
        static Node* internalCreateParticleSystem(const xmldrv::element&, ObjectLoader*);
    };

}

#endif

