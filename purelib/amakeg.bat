@echo off
set makefile="Android.mk"

set workdir=%cd%

echo workdir=%workdir%

rem write exist third_party static libraries
echo LOCAL_PATH := $(call my-dir)>%makefile%
echo. >>%makefile%
echo include $(CLEAR_VARS)>>%makefile%
echo LOCAL_SRC_FILES := prebuilt/libamagic.a>>%makefile%
echo LOCAL_MODULE := amagic>>%makefile%
echo include $(PREBUILT_STATIC_LIBRARY)>>%makefile%
echo.
echo. >>%makefile%
echo include $(CLEAR_VARS)>>%makefile%
echo $(call import-add-path,$(LOCAL_PATH)/../../cocos2d) >>%makefile%
echo $(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external) >>%makefile%
echo $(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos) >>%makefile%
echo $(call import-add-path,$(LOCAL_PATH)/../../cocos2d/extensions) >>%makefile%
echo.
echo.
echo LOCAL_MODULE := idskytdgame_shared>>%makefile%
echo LOCAL_MODULE_FILENAME := libidskytdgame>>%makefile%
echo. >>%makefile%
echo LOCAL_CPPFLAGS := -std=c++11 -DBOOST_ERROR_CODE_HEADER_ONLY  -DBOOST_SYSTEM_NO_DEPRECATED -pthread -frtti -fexceptions -fvisibility=hidden \>>%makefile%
echo -I../Classes/purelib \>>%makefile%
echo -I$(LOCAL_PATH)/../../cocos2d/extensions \>>%makefile%
echo -I$(LOCAL_PATH)/../../cocos2d/cocos/scripting/lua-bindings/manual \>>%makefile%
echo -I$(LOCAL_PATH)/../../cocos2d/external/lua/luajit/include \>>%makefile%
echo -I$(LOCAL_PATH)/../../cocos2d/external/lua/tolua>>%makefile%
echo. >>%makefile%

rem compile sources
call echo_nlf "LOCAL_SRC_FILES := hellocpp/main.cpp ">>%makefile%

rem call echo_nlf "../../Classes/purelib/NXVisibleRect.cpp " >>%makefile%

setlocal ENABLEEXTENSIONS
for /f "usebackq" %%i in (`search_cpp_f`) do (echo \>>%makefile%&&set buf=%%i&&call set buf=%%buf:%workdir%=../../Classes%%&&call set buf=%%buf:\=/%%&&call echo_nlf "%%buf%% " >>%makefile%)

rem for /f "usebackq" %%i in (`search_cpp_f`) do (echo \>>%makefile%&&call echo_nlf "../../Classes/%%~ni%%~xi " >>%makefile%)
setlocal DISABLEEXTENSIONS
echo. >>%makefile%
echo. >>%makefile%
echo. >>%makefile%
echo. >>%makefile%

echo LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes>>%makefile%
echo. >>%makefile%
rem ---- exists third_party libxxx.a ----
echo LOCAL_STATIC_LIBRARIES := amagic>>%makefile%
echo LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static cocostudio_static cocos_lua_static android_support>>%makefile%
echo. >>%makefile%
echo include $(BUILD_SHARED_LIBRARY)>>%makefile%
echo. >>%makefile%
echo $(call import-module,.) \>>%makefile%
echo $(call import-module,audio/android) \>>%makefile%
echo $(call import-module,extensions) \>>%makefile%
echo $(call import-module,editor-support/cocostudio) \>>%makefile%
echo $(call import-module,scripting/lua-bindings) \>>%makefile%
echo $(call import-module,android/support) >>%makefile%
echo. >>%makefile%


