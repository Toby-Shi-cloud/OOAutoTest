@echo off
set jar=code.jar
set test=5

echo You should put datainput_student_win64.exe, %jar%, and 1.in, 2.in, ... here in this folder.

cd ..
echo ---compiling utest2---
mingw32-make.exe utest2

echo ---compiling data---
mingw32-make.exe data

cd .\testdata\

echo ---running tests---

for /l %%i in (1,1,%test%) do (
..\data.exe 1>%%i.in
copy %%i.in stdin.txt
echo --%%i.in--
type %%i.in
 .\datainput_student_win64.exe | java -jar %jar% > %%i.out
 echo --%%i.out--
type %%i.out
echo --end--
 )

echo use mingw32-make.exe -f Makefile_English utest2 to use English output

echo ---Here is Your Result---
..\utest2.exe

pause
