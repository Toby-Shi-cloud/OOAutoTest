@echo off
set jar=code.jar
set test=5

echo You should put datainput_student_win64.exe, %jar%, and 1.in, 2.in, ... here in this folder.

echo running tests

for /l %%i in (1,1,%test%) do (copy %%i.in stdin.txt
 .\datainput_student_win64.exe | java -jar code.jar > %%i.out)


cd ..
mingw32-make.exe utest2
echo compiling utest2

echo ---Here is Your Result---
.\utest2.exe

pause