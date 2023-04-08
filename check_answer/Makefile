GCC := gcc
CC := g++
CFLAGS := -Wall $(DEFINES) -Wno-deprecated-declarations
CPPFLAGS := -std=c++11 $(CFLAGS)
CFLAGS += -std=c11
LDFLAGS := -lstdc++

ifeq ($(OS),Windows_NT)
	SHELL:=cmd
    RM = del /Q /F
    RRM = rmdir /Q /S
	LDFLAGS += -static
else
    RM = rm -f
    RRM = rm -f -r
endif

.PHONY: clean event.cpp checker.cpp

run: clean makeonly
	$(MAKE) clean_o
	./run

makeonly: data checker runtest.o
	$(CC) $(LDFLAGS) -o run runtest.o

runtest.o: runtest.c
	$(GCC) $(CFLAGS) -c runtest.c

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $<

data: rundata.o Data.o
	$(CC) $(LDFLAGS) -o data Data.o rundata.o

checker: runchecker.o event.o checker.o
	$(CC) $(LDFLAGS) -o checker runchecker.o event.o checker.o

clean_o:
	$(RM) *.o

clean: clean_o
ifeq ($(OS),Windows_NT)
	$(RM) run.exe data.exe checker.exe
	if exist ans $(RRM) ans
	if exist input $(RRM) input
else
	$(RM) run data checker
	$(RRM) ans
	$(RRM) input
endif
	$(RM) log.txt output.txt stdin.txt stdout.txt
