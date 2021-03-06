//
// Copyright (c) 2015 x-studio365 - All Rights Reserved.
//
#ifndef _VX_OBJECT_LOADER_H_
#define _VX_OBJECT_LOADER_H_
#include <unordered_map>
#include <functional>
#include <cocos2d.h>
#include "purelib/utils/xmldrv.h"
#include "purelib/utils/event_handler_manager.h"
USING_NS_CC;

namespace vx365 {

    enum EventType
    {
        kEventTypeTouchDown,
        kEventTypeTouchUp,
        kEventTypeTouchMoved,
        kEventTypeTouchCancelled,
        kEventTypeClick,
        kEventTypeDoubleClick,
        kEventTypeSceneLoading,
        kEventTypeTimeElapsed,
    };

    //typedef purelib::privacy::event_handler_manager<Node*, std::string, EventType, EventType> event_handler_manager;
    //typedef event_handler_manager::event_handler event_handler;

    class ObjectLoaderImpl;
    
    Node* findNode(Node* root, const char* name, bool recursively = true);
    Node* findNode(Node* root, int tag, bool recursively = true);

    class ObjectLoader
    {
        friend class ObjectLoaderImpl;

    public:
        Node*              load(const char* file, bool security = true, const char* key = nullptr);

        Node*              findNode(const char* xkey);
        std::vector<Node*> findNodes(const char* xkey);
        //void       registerVariable(const std::string& xkey, Node** ppNodeStorage);


        // xkey
        // supported format: 
        //   1. /Scene/LayerColor_1/Sprite_1
        //
        //template<typename _Fty>
        //event_handler* registerHandler(const std::string& xkey, EventType eventType, const _Fty& callback)
        //{
        //    auto handler = event_handler_manager::new_event_handler<_Fty>(callback);
        //    handler->set_event_type(eventType);
        //    return ehm_.register_handler(xkey, handler);
//}

        //void       unregisterHandler(const std::string& xpath);
        //void       unregisterHandler(event_handler* handler);

        bool       isMergedTexUsed(void) const { return merged_tex_used_; }
    private:
        std::string                                  currentkey_;
        //event_handler_manager                   ehm_;
        std::unordered_multimap<std::string, Node*>  search_tab_;
        bool                                         merged_tex_used_;
    };
}

#endif

