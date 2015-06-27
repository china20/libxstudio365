/***
**     Copyright (c) 2014 purelib. All rights reserved.
**
*******************************************************************************/
#include <platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h>
#include "NXUserDefault.h"
#include "purelib/platform/NXNativeUtils.h"
#include "purelib/utils/crypto_wrapper.h"
#include "purelib/utils/xxfsutility.h"
#include <unistd.h>

#define JSON_FILE_NAME "local_settings.dat"

static bool lcgIsEncrypted = true;

#define ENCRYPT_KEY "E,wrJbrQkqVS5ujyVCPhTWdeCBI0uAqU"
#define ENCRYPT_KEY_FRAG "bckS15JlaW"

namespace purelib {
const UserDefault::value_type UserDefault::empty;
UserDefault::UserDefault(void)
{
    this->initialize();
}

bool UserDefault::initialize(void)
{
	initKey();

    // UserDefault.xml is stored in /data/data/<package-path>/ before v2.1.2
    this->path += "/data/data/" + getPackageNameJNI() + "/" + JSON_FILE_NAME;
    
    if(isJsonFileExist())
    {
        return this->loadJsonFile();
    }
    else {
        return this->createJsonFile();
    }
}


void UserDefault::initKey(void)
{
	//std::string deviceId = getIMEI() + ENCRYPT_KEY_FRAG;
	//crypto::aes::pkcs(deviceId);
	//CCLOG("device id:%s, size:%u", deviceId.c_str(), deviceId.size());
	//crypto::aes::encrypt_v2(deviceId, this->encrypt_key, ENCRYPT_KEY);
	//CCLOG("encrypt key size:%u", this->encrypt_key.size());
	this->encrypt_key = NXNativeUtils::getDeviceId();
	crypto::aes::overlapped::encrypt<crypto::aes::CipherMode::ECB, crypto::aes::PaddingMode::None>(this->encrypt_key);
}

bool UserDefault::loadJsonFile(void)
{
    fsutil::read_file_data(this->path.c_str(), this->encrypted_buffer);
    
    if(encrypted_buffer.empty())
    {
        CCLOG("UserDefault::initizlize: can not read json file!");
        return false;
    }

    return true;
}

bool UserDefault::createJsonFile(void)
{
    value_type root;
	this->repack(root);
	this->flush();
    return true;
}


bool UserDefault::isJsonFileExist()
{

    FILE *fp = fopen(this->path.c_str(), "r");
    bool bRet = false;

    if (fp)
    {
        bRet = true;
        fclose(fp);
    }

    return bRet;
}

void UserDefault::insert(const char* key, const UserDefault::value_type& value, bool flushImmediately )
{
    value_type root;
	parser_type parser;
	this->unpack(root, parser);

#ifdef _USING_JSONCPP
    root[key] = value;
#else
	root.base().AddMember(key, const_cast<rapidjson::Value&>(value.base()), parser.GetAllocator());
#endif

    this->repack(root);

	if (flushImmediately)
		this->flush();
}

//UserDefault::value_type& UserDefault::getRef(const char* key)
//{
//    return this->buffer[key];
//}

UserDefault::value_type UserDefault::get(const char* key, const UserDefault::value_type& defaultValue) const
{
#ifdef _USING_JSONCPP
    value_type root;
	parser_type parser;
	if (this->unpack(root, parser))
    {
		return root.get(key, defaultValue);
    }
    return defaultValue;
#else
	value_type root;
	auto autoParser = CCRJParser::create();
	if (this->unpack(root, autoParser->d_))
	{
		return root[key];
	}
	return defaultValue;
#endif
}

/**
@brief erase value by key, if the key doesn't exist, do nothing.
*/
void UserDefault::erase(const char* key, bool flushImmediately)
{
    value_type root;
	parser_type parser;
#ifdef _USING_JSONCPP
	if (this->unpack(root, parser) && root.removeMember(key) != CSJson::nullValue)
    {
        this->repack(root);
		if (flushImmediately)
			this->flush();
    }
#else
	if (this->unpack(root, parser ) && root.base().RemoveMember(key))
	{
		this->repack(root);
		if (flushImmediately)
			this->flush();
	}
#endif
}

bool UserDefault::contains(const char* key) const
{
    value_type root;
	parser_type parser;
	CC_ASSERT(this->unpack(root, parser));

#ifdef _USING_JSONCPP
    return root.isMember(key);
#else
	return root.base().HasMember(key);
#endif
}

bool UserDefault::unpack(value_type& root, parser_type& parser) const
{
	if (lcgIsEncrypted) {
		std::string buffer = crypto::aes::decrypt(this->encrypted_buffer, this->encrypt_key.c_str());
#ifdef _USING_JSONCPP
		return parser.parse(buffer.c_str(), root, false);
#else
		return !parser.Parse<1>(buffer.c_str()).IsNull();
#endif
	}
	else {
#ifdef _USING_JSONCPP
		return parser.parse(encrypted_buffer.c_str(), root, false);
#else
		return !parser.Parse<1>(encrypted_buffer.c_str()).IsNull();
#endif
	}
}

void UserDefault::repack(const value_type& root)
{
#ifdef _USING_JSONCPP
	this->encrypted_buffer = root.toStyledString();
#else
	this->encrypted_buffer = root.to_string();
#endif
	if (lcgIsEncrypted)
		crypto::aes::overlapped::encrypt(encrypted_buffer, this->encrypt_key.c_str());
}

void UserDefault::flush(void)
{
    FILE* fp = 0;
#if defined(_MSC_VER) && (_MSC_VER >= 1400 ) && (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
    errno_t err = fopen_s(&fp, this->path.c_str(), "wb+" );
    if ( !fp || err) {
#else
    fp = fopen( this->path.c_str(), "wb+" );
    if ( !fp) {
#endif
        CCLOG("UserDefault::flush: Open file failed!");
        // return false;
    }

    size_t bytesWritten = 0;
	bytesWritten = fwrite(this->encrypted_buffer.c_str(), this->encrypted_buffer.length(), 1, fp);

    fclose(fp);
    // return true;
}

};


