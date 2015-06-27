/***
** Module: INSUserDefault -- singleton
**
**     Copyright (c) 2014 purelib. All rights reserved.
**
** Purpose: Save Game
**
** Usage: singleton<INSUserDefault>::instance()
**
*******************************************************************************/
#ifndef _NXUSERDEFAULT_H_
#define _NXUSERDEFAULT_H_
#include <utility>
#include "NXGameDefs.h"
#include "utils/nsconv.h"

#undef _USING_JSONCPP
#undef _USING_RAPIDJSON
#define _USING_JSONCPP

#ifdef _USING_JSONCPP
#include "jsoncpp/json_lib.h"
#else
#include "utils/json_wrapper.h"

class CCRJParser : public Ref
{
public:
    CREATE_FUNC(CCRJParser);
    bool init(){return true;};
public:
    rapidjson::Document d_;
    std::string buffer_;
};

#endif


namespace purelib {

class UserDefault
{
public:
    UserDefault(void);
#ifdef _USING_JSONCPP
    typedef CSJson::Value value_type;
    typedef CSJson::Reader parser_type;
#else
    typedef jsonwrapper::object value_type;
    typedef rapidjson::Document parser_type;
#endif
    static const value_type empty;

    /**
    @brief set value by key, if the key exist, update, else add new.
    */
    void insert(const char* key, const value_type& value, bool flushImmediately = false);

     /**
    @brief get value by key, if the key doesn't exist, return the default value.
    */
    value_type get(const char* key, const value_type& defaultValue = empty) const;

    /**
    @brief erase value by key, if the key doesn't exist, do nothing.
    */
    void erase(const char* key, bool flushImmediately = false);

    /**
    @brief whether contains key, if the key doesn't exist, do nothing.
    */
    bool contains(const char* key) const;

    /**
    @brief flush data to device's disk immediately.
    */
    void       flush(void);

public:
    ///  only for array, 
    ///  has value: [0,n), no value: -1
    /*static int indexOf(const value_type& original, const char* key, int value)
    {
        if (original.size() == 0 && original.type() != CSJson::arrayValue)
            return -1;

        int idx = 0;
        for (CSJson::ValueIterator iter = original.begin(); iter != original.end(); ++iter, ++idx)
        {
            if ((*iter).isMember(key))
            {
                if ((*iter)[key].asInt() == value)
                {
                    return idx;
                }
            }
        }
        return -1;
    }

    /// Only for array
    static value_type valueOf(const value_type& original, const char* key, int value)
    {
        int idx = indexOf(original, key, value);
        if (idx != -1) {
            return original[idx];
        }
        return empty;
    }*/

private:

    bool       unpack(value_type& root, parser_type& parser) const;

    void       repack(const value_type& root);

    bool       initialize(void);
    void       initKey(void);
    bool       loadJsonFile(void);
    bool       createJsonFile(void);
    
    bool       isJsonFileExist(void);

private:
    std::string            encrypted_buffer;
    std::string            path;
    std::string            encrypt_key;
};

};

#define userdef thelib::gc::singleton<purelib::UserDefault>::instance()

#endif

