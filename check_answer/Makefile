CC := g++
CFLAGS := -Wall $(DEFINES) -Wno-deprecated-declarations
CPPFLAGS := -std=c++11 $(CFLAGS)
CFLAGS += -std=c11
LDFLAGS := -lstdc++
ifeq ($(shell uname), Darwin)
	LDFLAGS += 
else
	LDFLAGS += -static
endif

.PHONY: clean event.cpp checker.cpp

run: data checker runtest.o
	$(CC) $(LDFLAGS) -o run runtest.o
	$(MAKE) clean_o
	./run

runtest.o: runtest.c
	gcc $(CFLAGS) -c runtest.c

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $<

data: rundata.o data.o
	$(CC) $(LDFLAGS) -o data data.o rundata.o

checker: runchecker.o event.o checker.o
	$(CC) $(LDFLAGS) -o checker runchecker.o event.o checker.o

clean_o:
	rm -f *.o

clean: clean_o
	rm -f run data checker 
	rm -rf ans
	rm -rf input
	rm log.txt output.txt stdin.txt
