//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#include "VXObjectLoader.h"
#include "VXObjectLoaderImpl.h"

using namespace x365;

Node* ObjectLoader::load(const char* file, bool security, const char* key)
{
    merged_tex_used_ = false;

    if (key != nullptr)
        xmldrv::set_encrypt_key(key);
    xmldrv::set_security(security);
    xmldrv::document doc(file);
    if (!doc.is_open())
        return nullptr;

    merged_tex_used_ = doc.root().get_attribute_value("merged_tex_used", false);

    return ObjectLoaderImpl::convertFromRecursively(doc, this);
}

void ObjectLoader::registerVariable(const std::string& xkey, Node** ppNodeStorage)
{
	this->variable_tab_.insert(std::make_pair(xkey, ppNodeStorage));
}

void ObjectLoader::unregisterHandler(const std::string& xpath)
{
    ehm_.unregister_handler(xpath);
}

void ObjectLoader::unregisterHandler(event_handler* handler)
{
    ehm_.unregister_handler(handler);
}
