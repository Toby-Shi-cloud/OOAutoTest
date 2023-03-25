@echo off
set jar=hw5.jar
set test=5

cd ..
echo ---compiling utest2---
mingw32-make.exe utest2

cd .\testdata\

echo ---running tests---

for /l %%i in (1,1,%test%) do (echo --%%i.in--
type %%i.in
echo --%%i.out--
type %%i.out
echo --end--
)

echo ---Here is Your Result---
..\utest2.exe

pause