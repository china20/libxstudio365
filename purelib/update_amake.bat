@echo off
cd Classes\
call amakeg.bat
move /y Android.mk ..\proj.android\jni\
if %errorlevel% neq 0 echo update android makefile failed! && ping /n 3 127.0.1 >nul && goto :eof
echo update android makefile succeed.

ping /n 3 127.0.1 >nul && goto :eof

