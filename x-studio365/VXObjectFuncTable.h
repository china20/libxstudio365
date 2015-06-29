//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#ifndef _VXOBJECTFUNCTABLE_H_
#define _VXOBJECTFUNCTABLE_H_

#include <utility>
#include "VXEnumTypeInfo.h"
#include "purelib/utils/xmldrv.h"

namespace cocos2d {
    class Node;
};

namespace vx365 {
    
    class ObjectLoader;
};

typedef cocos2d::Node* (*LPFUNC_CREATE_OBJECT)(const xmld::element&, vx365::ObjectLoader*);
typedef void(*LPFUNC_LOAD_OBJECT_ATTR)(const xmld::element&, cocos2d::Node*);

struct VXFuncTableItem
{
    VXObjectType            type;
    LPFUNC_CREATE_OBJECT    lpfnCreateObject;
    LPFUNC_LOAD_OBJECT_ATTR lpfnLoadObjectAttr;
};

#endif



