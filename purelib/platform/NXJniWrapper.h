#ifndef _NXJNIWRAPPER_H_
#define _NXJNIWRAPPER_H_
#include "platform/android/jni/JniHelper.h"

#include <android/log.h>
#include <string>
#include <map>
#include <jni.h>

class NXJniWrapper {

public:
	typedef std::map<std::string, std::string> JniParamTable;

	static void        setJavaClass(const std::string& className);

	static bool        setJavaObjectRef(const char* funcName);
	static void        clearJavaObjectRef(void);

	static void        callJavaStaticFunc(const char* funcName, const double param);
    static void        callJavaStaticFunc(const char* funcName, const std::string& param);
    static void        callJavaStaticFunc(const char* funcName, const JniParamTable& paramTable);

    static int         callJavaStaticIntFunc(const char* funcName);
    static double      callJavaStaticDoubleFunc(const char* funcName);
    static std::string callJavaStaticStringFunc(const char* funcName);

    // call current java object's methods

    static void        callJavaObjectFunc(const char* funcName);

    static void        callJavaObjectFunc(const char* funcName, ssize_t param);
};

#endif

