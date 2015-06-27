/***
**     Copyright (c) 2014 purelib. All rights reserved.
**
*******************************************************************************/

#include "utils/crypto_wrapper.h"
#include "utils/oslib.h"

#include "NXSecurityDataTable.h"

static const SecurityData lgEmptySecurityData;

#define CONVERT_BOOL(from) static_cast<bool>( atoi(from.c_str()) )
#define CONVERT_INT(from) atoi(from.c_str())
#define CONVERT_LARGE_INT(from) atoll(from.c_str())
#define CONVERT_FLOAT(from) static_cast<float>( atof(from.c_str()) )
#define CONVERT_DOUBLE(from) atof(from.c_str())
#define CONVERT_STRING(from) (from)

namespace purelib {

_INTKEY_HASH_IMPL(SecurityDataTable, Bool, bool, SecurityDataTypeBool, CONVERT_BOOL);
_INTKEY_HASH_IMPL(SecurityDataTable, Int, int, SecurityDataTypeInt, CONVERT_INT);
_INTKEY_HASH_IMPL(SecurityDataTable, LargestInt, long long, SecurityDataTypeLargestInt, CONVERT_LARGE_INT);
_INTKEY_HASH_IMPL(SecurityDataTable, Float, float, SecurityDataTypeFloat, CONVERT_FLOAT);
_INTKEY_HASH_IMPL(SecurityDataTable, Double, double, SecurityDataTypeDouble, CONVERT_DOUBLE);
_INTKEY_HASH_IMPL(SecurityDataTable, String, std::string, SecurityDataTypeString, CONVERT_STRING);

SecurityDataTable::SecurityDataTable(void) : theId(0)
{
}

void  purelib::SecurityDataTable::cleanup(void) // 清除所有加密数据
{
    this->theId = 0;
    this->table.clear();
}

};

