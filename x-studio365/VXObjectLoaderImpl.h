//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#ifndef _VX_OBJECT_LOADER_IMPL_H_
#define _VX_OBJECT_LOADER_IMPL_H_

#include <unordered_map>
#include <functional>
#include <cocos2d.h>
#include "VXEnumTypeInfo.h"
#include "purelib/utils/xmldrv.h"
USING_NS_CC; 

namespace x365 {

    class ObjectLoader;

    class ObjectLoaderImpl
    {
        PLIB_STATICIZE_CLASS(ObjectLoaderImpl);
    public:

        static Node*  convertFromRecursively(const xml4w::document&, ObjectLoader* context);
        static void   convertFromRecursively(const xml4w::element&, Node* parent, ObjectLoader* context, int deep = 1);

        static void   convertFromRecursivelyImpl(const xml4w::element& levelInfo, cocos2d::Node* levelNode, ObjectLoader* context, int deep);

		static Node*  createObjectInternal(const xml4w::element& levelInfo, cocos2d::Node* parent, VXObjectType type, ObjectLoader* context);
		static Node*  createObjectInternal(const xml4w::element& levelInfo, VXObjectType type, ObjectLoader* context);

        static Node*  createNode(const xml4w::element& levelInfo, ObjectLoader* context);
    };

}

#endif

