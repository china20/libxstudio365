//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#include "NXSharedPtr.h"
#include "NXMacroDefs.h"
#include "NXLangUtility.h"

/* @brief: util language, language Adaptive
    ** @remark: support en-us and zh-cn
    */
const std::string lang_utility::text(const char * key)
{
    // Get current language type
    LanguageType languageType = CCAPP->getCurrentLanguage();
    
    const char* langPath = "data/lang/zh-cn.plist";
    switch(languageType)
    {
    case LanguageType::ENGLISH:
        langPath = "data/lang/en-us.plist"; // CCDictionary::createWithContentsOfFile("data/lang/en-us.plist");
        break;
    default: ;
    }

    static ValueMap valueMap = CCFILEUTILS->getValueMapFromFile(langPath);

    auto target = valueMap.find(key);
    if(target != valueMap.end()) {
        return target->second.asString();
    }

    return key;
}

