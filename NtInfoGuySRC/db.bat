@echo off
rem **********************************************************************
rem 				NT Driver Guy
rem		Made By ����|hopy 2009-01-17 @ HFE_Airport
rem blog	: http://blog.csdn.net/mydo
rem email 	: k26s@hotmail.com
rem **********************************************************************

cls

echo NT DDK DRIVER BUILDER 2010-06-17 by ����@hopy

rem ����DDK�İ�װ·��
set DDK_PATH=d:\code\WINDDK\3790.1830

if (%1) == (sc) goto SET_CHK
if (%1) == (sf) goto SET_FREE
if (%1) == (c) goto CHK
if (%1) == (f) goto FREE
if (%1) == (h) goto HELP

echo Unknow KONB , Try To : db h
goto EXIT

:HELP
echo "���� DDK PART ʹ�÷���  db [sc|sf|c|f|h]"
echo sc : 	����DDK����汾Ϊcheck
echo sf : 	����DDK����汾Ϊfree
echo c  : 	��check�汾������Ŀ
echo f  : 	��free�汾������Ŀ
echo h  : 	��ӡ������Ϣ
echo ���� NT GCC OBJECT ��ο����� b h
goto EXIT
rem color 0c

:SET_CHK
set CD_PATH=%cd%
call %DDK_PATH%\bin\setenv.bat %DDK_PATH% chk WXP
cd %CD_PATH%
echo Set NtDriverGuy Object to CHK Mode
goto EXIT

:CHK
set BUILD_KONB=/Od /Gz /c /W3 /D_X86_
echo Build With CHK
goto NEXT

:SET_FREE
set CD_PATH=%cd%
call %DDK_PATH%\bin\setenv.bat %DDK_PATH% fre WXP
cd %CD_PATH%
echo Set NtDriverGuy Object to FREE Mode
goto EXIT

:FREE
set BUILD_KONB=/Ox /Gz /c /W3 /D_X86_
echo Build With FREE
goto NEXT

:NEXT
cl %BUILD_KONB% /I %DDK_PATH%\inc\ddk\wxp /I %DDK_PATH%\inc\crt /TC /FoDDK_PART\DDK_C.obj DDK_PART\DDK_C.c

:EXIT