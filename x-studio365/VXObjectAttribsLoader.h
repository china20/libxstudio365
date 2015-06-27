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

namespace x365 {

    class ObjectLoader;

    class ObjectAttribsLoader
    {
		PLIB_STATICIZE_CLASS(ObjectAttribsLoader);
    public:

		static void loadObjectAttribs(const xmldrv::element&, VXObjectType objType, Node*);

		static void loadAttribsOfNode(const xmldrv::element&, Node*);
		static void loadAttribsOfSprite(const xmldrv::element&, Node*);
		static void loadAttribsOfScale9Sprite(const xmldrv::element&, Node*);
		static void loadAttribsOfLabel(const xmldrv::element&, Node*);
		static void loadAttribsOfLabelAtlas(const xmldrv::element&, Node*);
		static void loadAttribsOfLabelBMFont(const xmldrv::element&, Node*);
		static void loadAttribsOfParticleSystem(const xmldrv::element&, Node*);

		static void loadAttribsOfTextField(const xmldrv::element&, Node*);
		static void loadAttribsOfButton(const xmldrv::element&, Node*);
		static void loadAttribsOfSliderBar(const xmldrv::element&, Node*);
		static void loadAttribsOfProgressBar(const xmldrv::element&, Node*);
		static void loadAttribsOfCheckBox(const xmldrv::element&, Node*);
		static void loadAttribsOfSwitch(const xmldrv::element&, Node*);

		static void loadAttribsOfLayout(const xmldrv::element&, Node*);
		static void loadAttribsOfScrollView(const xmldrv::element&, Node*);
		static void loadAttribsOfPageView(const xmldrv::element&, Node*);
		static void loadAttribsOfListView(const xmldrv::element&, Node*);
    };

}

#endif

