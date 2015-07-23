//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#include "VXObjectLoader.h"
#include "VXObjectLoaderImpl.h"

using namespace vx365;

Node* vx365::findNode(Node* root, const char* name, bool recursively )
{
    if (root == nullptr)
        return nullptr;

    // find this node
    auto target = root->getChildByName(name);
    if (target != nullptr)
        return target;

    if (recursively)
        // recursively
        for (auto& child : root->getChildren())
        {
            auto target = findDescendant(child, name, recursively);
            if (target != nullptr)
                return target;
        }

    return nullptr;
}

Node* vx365::findNode(Node* root, int tag, bool recursively )
{
    if (root == nullptr)
        return nullptr;

    // find this node
    auto target = root->getChildByTag(tag);
    if (target != nullptr)
        return target;

    if (recursively)
        // recursively
        for (auto& child : root->getChildren())
        {
            auto target = findDescendant(child, tag, recursively);
            if (target != nullptr)
                return target;
        }

    return nullptr;
}

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

#if 0
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

#endif
