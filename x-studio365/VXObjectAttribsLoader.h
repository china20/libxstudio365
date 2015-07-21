//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#ifndef _X365_OBJECT_ATTRIBS_LOADER_H_
#define _X365_OBJECT_ATTRIBS_LOADER_H_

#include <unordered_map>
#include <functional>
#include <cocos2d.h>
#include "VXEnumTypeInfo.h"
#include "purelib/utils/xmldrv.h"
USING_NS_CC;

namespace vx365 {

    class ObjectLoader;

    class ObjectAttribsLoader
    {
        PLIB_STATICIZE_CLASS(ObjectAttribsLoader);
    public:

        static void loadObjectAttribs(const xmldrv::element&, VXObjectType objType, Node*, ObjectLoader* ctx);

        static void loadAttribsOfNode(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfSprite(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfScale9Sprite(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfLabel(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfLabelAtlas(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfLabelBMFont(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfParticleSystem(const xmldrv::element&, Node*, ObjectLoader* ctx);

        static void loadAttribsOfTextField(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfButton(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfSliderBar(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfProgressBar(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfCheckBox(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfSwitch(const xmldrv::element&, Node*, ObjectLoader* ctx);

        static void loadAttribsOfLayout(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfScrollView(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfPageView(const xmldrv::element&, Node*, ObjectLoader* ctx);
        static void loadAttribsOfListView(const xmldrv::element&, Node*, ObjectLoader* ctx);
    };

}

#endif

