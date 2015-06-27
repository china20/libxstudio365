/***
** Module: nx SecurityDataTable -- singleton
**
**     Copyright (c) 2014 purelib. All rights reserved.
**
** Purpose: Save Game, 安全加密数据表 Save and Load Global Variables
**
** Usage: singleton<SecurityDataTable>::instance() or macro hashtable
**
*******************************************************************************/
#ifndef _NX_SECURITYDATATABLE_H_
#define _NX_SECURITYDATATABLE_H_
#include <string>
#include <unordered_map>
#include "utils/singleton.h"
#include "utils/nsconv.h"

using namespace purelib;
using namespace purelib::gc;

typedef unsigned long long SecurityDataId;

static const SecurityDataId INVALID_SECURITYDATAID = (SecurityDataId)~0;

extern const char* udg_data_secret_key; 

typedef enum {
    SecurityDataTypeUndefined,
    SecurityDataTypeBool,
    SecurityDataTypeInt,
    SecurityDataTypeLargestInt,
    SecurityDataTypeFloat,
    SecurityDataTypeDouble,
    SecurityDataTypeString,
} SecurityDataType;

struct SecurityData
{
    SecurityData(void) : type(SecurityDataTypeUndefined)
    {
    }
    SecurityData(const int& value) : type(SecurityDataTypeInt)
    {
        stream = nsc::to_string(value);
    }
    SecurityData(const long long& value) : type(SecurityDataTypeLargestInt)
    {
        stream = nsc::to_string(value);
    }
    SecurityData(const float& value) : type(SecurityDataTypeFloat)
    {
        stream = nsc::to_string(value);
    }
    SecurityData(const double& value) : type(SecurityDataTypeDouble)
    {
        stream = nsc::to_string(value);
    }
    SecurityData(const std::string& value) : type(SecurityDataTypeString)
    {
        stream = value;
    }
    SecurityData& operator=(const int& value) 
    {
        type = SecurityDataTypeInt;
        stream = nsc::to_string(value);
        return *this;
    }
    SecurityData& operator=(const long long& value) 
    {
        type = SecurityDataTypeLargestInt;
        stream = nsc::to_string(value);
        return *this;
    }
    SecurityData& operator=(const float& value)
    {
        type = SecurityDataTypeFloat;
        stream = nsc::to_string(value);
        return *this;
    }
    SecurityData& operator=(const double& value)
    {
        type = SecurityDataTypeDouble;
        stream = nsc::to_string(value);
        return *this;
    }
    SecurityData& operator=(const std::string& value)
    {
        type = SecurityDataTypeString;
        stream = value;
        return *this;
    }

    SecurityDataType type;
    std::string      stream;
};

#define _SECURITY_HASH_DECL(name,keyty,valuety) \
    public: \
    const keyty save##name(const valuety&); \
    const keyty save##name(const keyty&, const valuety&); \
    const valuety loadSaved##name(const keyty&, const valuety& defaultValue); \
    void removeSaved##name(const keyty&); \
    private: \

#define _SECURITY_HASH_IMPL(classname,name,keyty,valuety,tyid,converter) \
    const keyty classname ::save##name(const valuety& value) \
{ \
    return this->save##name(++this->theId, value);\
} \
    const keyty classname ::save##name(const keyty& identifier, const valuety& value) \
{ \
    if(identifier == INVALID_SECURITYDATAID || 0 == identifier)\
{\
    return INVALID_SECURITYDATAID;\
}\
    auto target = this->table.find(identifier);\
    SecurityData* _op = nullptr;\
    if(target != this->table.end())\
{\
    _op = &target->second;\
}\
        else {\
        auto result = this->table.insert(std::make_pair(identifier, lgEmptySecurityData));\
        assert(result.second);\
        _op = &(result.first->second);\
}\
    assert(_op != nullptr);\
    if(SecurityDataTypeUndefined == _op->type || tyid == _op->type) \
{\
    *_op = value;\
    crypto::aes::pkcs(_op->stream);\
    crypto::aes::ecb_encrypt(_op->stream.c_str(), _op->stream.size(), &_op->stream.front(), _op->stream.size(), udg_data_secret_key, 128);\
    return identifier;\
}\
    return INVALID_SECURITYDATAID;\
} \
    const valuety classname ::loadSaved##name(const keyty& id, const valuety& defaultValue) \
{ \
    auto target = this->table.find(id);\
    if(target != this->table.end())\
{\
    if(target->second.type == tyid) {\
    std::string result;\
    crypto::aes::ecb_decrypt(target->second.stream.c_str(), target->second.stream.size(), result, udg_data_secret_key, 128);\
    return converter(result); \
    }\
}\
    return defaultValue;\
} \
    void classname ::removeSaved##name(const keyty& key) \
{ \
    this->table.erase(key); \
} 

#define _INTKEY_HASH_DECL(name,valuety)                _SECURITY_HASH_DECL(name, SecurityDataId, valuety)
#define _INTKEY_HASH_IMPL(classname,name,valuety,tyid,converter) _SECURITY_HASH_IMPL(classname, name, SecurityDataId, valuety, tyid, converter)

namespace purelib {

class SecurityDataTable
{
public:
    SecurityDataTable(void);

    _INTKEY_HASH_DECL(Bool, bool);
    _INTKEY_HASH_DECL(Int, int);
    _INTKEY_HASH_DECL(LargestInt, long long);
    _INTKEY_HASH_DECL(Float, float);
    _INTKEY_HASH_DECL(Double, double);
    _INTKEY_HASH_DECL(String, std::string);


    void             cleanup(void); // 清除所有加密数据

private:
    std::unordered_map<SecurityDataId, SecurityData> table;
    SecurityDataId                                   theId;
};

};

#define shashtable singleton<purelib::SecurityDataTable>::instance()

#endif

