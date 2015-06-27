#include "NXJniWrapper.h"

#define  LOG_TAG    "NXJniWrapper"
//#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
//#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


// using namespace ingl::intri;
using namespace cocos2d;

static std::string theJavaClassName = "com/thisapp/lib/utils/NXNativeUtils";

static jobject internalCurrentJavaObject;

static JNIEnv* internalGetEnv()
{
	auto jvm = JniHelper::getJavaVM();
    bool bRet = false;
    JNIEnv* env = NULL;
    do
    {
		if (jvm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
        {
        	//outputLog("PluginUtils", "Failed to get the environment using GetEnv()");
            break;
        }

        if (jvm->AttachCurrentThread(&env, 0) < 0)
        {
            //outputLog("PluginUtils", "Failed to get the environment using AttachCurrentThread()");
            break;
        }

        bRet = true;
    } while (0);

    if (!bRet) {
    	env = NULL;
    }

    return env;
}

void NXJniWrapper::setJavaClass(const std::string& className)
{
	theJavaClassName = className;
}

bool NXJniWrapper::setJavaObjectRef(const char* funcName)
{
	LOGE("%s", "NXJniWrapper::setJavaObjectRef...");
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, theJavaClassName.c_str(), funcName,
			"()Ljava/lang/Object;")) {

		internalCurrentJavaObject = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		//char* cstr = (char*) t.env->GetStringUTFChars(jstr, nullptr);
		//std::string ret(cstr);

		//t.env->ReleaseStringUTFChars(jstr, cstr);
		//t.env->DeleteLocalRef(jstr);

		t.env->DeleteLocalRef(t.classID);

		LOGE("NXJniWrapper::setJavaObjectRef sucess, SDK Instance:%p...", internalCurrentJavaObject);
		return true;
		//return std::move(ret);
	}

	LOGE("%s", "Get IngleSdk Instance Failed!");

	return  false;
}

void NXJniWrapper::clearJavaObjectRef(void)
{
	auto env = internalGetEnv();
	env->DeleteLocalRef(internalCurrentJavaObject);
}

static jobject internalConvertToJavaHashtable(const NXJniWrapper::JniParamTable& paramMap) {
	JNIEnv* env = internalGetEnv();
	jclass class_Hashtable = env->FindClass("java/util/Hashtable");
	jmethodID construct_method = env->GetMethodID(class_Hashtable, "<init>",
			"()V");
	jobject obj_Map = env->NewObject(class_Hashtable, construct_method, "");
	//if (paramMap != NULL) {
		jmethodID add_method = env->GetMethodID(class_Hashtable, "put",
				"(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		for (auto it = paramMap.begin(); it != paramMap.end(); ++it) {
			jstring first = env->NewStringUTF(it->first.c_str());
			jstring second = env->NewStringUTF(it->second.c_str());
			env->CallObjectMethod(obj_Map, add_method, first, second);
			env->DeleteLocalRef(first);
			env->DeleteLocalRef(second);
		}
	//}
	env->DeleteLocalRef(class_Hashtable);
	return obj_Map;
}

void NXJniWrapper::callJavaStaticFunc(const char* funcName, const double param)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, theJavaClassName.c_str(), funcName, "(D)V")) {

		t.env->CallStaticVoidMethod(t.classID, t.methodID, param);

		t.env->DeleteLocalRef(t.classID);
	}
}

void NXJniWrapper::callJavaStaticFunc(const char* funcName, const std::string& arg) {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, theJavaClassName.c_str(), funcName,
			"(Ljava/lang/String;)V")) {
		jstring jarg = t.env->NewStringUTF(arg.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jarg);

		t.env->DeleteLocalRef(jarg);
		t.env->DeleteLocalRef(t.classID);
	}
}

void NXJniWrapper::callJavaStaticFunc(const char* funcName, const JniParamTable& paramTable)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, theJavaClassName.c_str(), funcName, "(Ljava/util/Hashtable;)V"))
	{
		auto obj_Map = internalConvertToJavaHashtable(paramTable);

		// invoke java method
		t.env->CallStaticVoidMethod(t.classID, t.methodID, obj_Map);
		t.env->DeleteLocalRef(obj_Map);
		t.env->DeleteLocalRef(t.classID);
	}
}

int NXJniWrapper::callJavaStaticIntFunc(const char* funcName)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, theJavaClassName.c_str(), funcName, "()I")) {
		auto value = t.env->CallStaticIntMethod(t.classID, t.methodID);
		return value;
	}

	return 0;
}

double NXJniWrapper::callJavaStaticDoubleFunc(const char* funcName)
{
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, theJavaClassName.c_str(), funcName, "()D")) {
		auto value = t.env->CallStaticDoubleMethod(t.classID, t.methodID);
		return value;
	}

	return 0;
}

std::string NXJniWrapper::callJavaStaticStringFunc(const char* funcName) {
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, theJavaClassName.c_str(), funcName,
			"()Ljava/lang/String;")) {
		jstring jstr = (jstring) t.env->CallStaticObjectMethod(t.classID,
				t.methodID);

		char* cstr = (char*) t.env->GetStringUTFChars(jstr, nullptr);
		std::string ret(cstr);

		t.env->ReleaseStringUTFChars(jstr, cstr);
		t.env->DeleteLocalRef(jstr);

		t.env->DeleteLocalRef(t.classID);

		return std::move(ret);
	}

	return "";
}

void  NXJniWrapper::callJavaObjectFunc(const char* funcName)
{
	LOGE("NXJniWrapper::callJavaObjectFunc, funcName:%s", funcName);
	JniMethodInfo t;
	if (JniHelper::getMethodInfo(t
            , theJavaClassName.c_str()
            , funcName
            , "()V"))
	{
		t.env->CallVoidMethod(internalCurrentJavaObject, t.methodID);
		t.env->DeleteLocalRef(t.classID);

		LOGE("NXJniWrapper::callJavaObjectFunc successfully, funcName:%s", funcName);
	}
	else
		LOGE("NXJniWrapper::callJavaObjectFunc failed, funcName:%s", funcName);
}

void NXJniWrapper::callJavaObjectFunc(const char* funcName, ssize_t param)
{
	LOGE("NXJniWrapper::callJavaObjectFunc with ssize_t param, funcName:%s", funcName);
		JniMethodInfo t;
		if (JniHelper::getMethodInfo(t
	            , theJavaClassName.c_str()
	            , funcName
	            , "(J)V"))
		{
			t.env->CallVoidMethod(internalCurrentJavaObject, t.methodID, (jlong)param);
			t.env->DeleteLocalRef(t.classID);

			LOGE("NXJniWrapper::callJavaObjectFunc with ssize_t param successfully, funcName:%s", funcName);
		}
		else
			LOGE("NXJniWrapper::callJavaObjectFunc with ssize_t param failed, funcName:%s", funcName);
}

/*extern "C" {
    JNIEXPORT void JNICALL Java_com_thisapp_lib_utils_NXNativeUtils_nativeInitJNI(JNIEnv*  env, jobject thiz, jobject context, jobject instance) {
    	internalCurrentJavaObject = env->NewGlobalRef(instance);
    	ingl::intri::JniHelper::setClassLoaderFrom(context);
    	LOGE("nativeInitJNI: context:%p, sdk instance: %p", context, instance);
    	LOGE("nativeInitJNI: internalCurrentJavaObject: %p", internalCurrentJavaObject);
    }
}*/


