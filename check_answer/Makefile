CC := g++
CFLAGS := -Wall -Werror -std=c++11 $(DEFINES)
LDFLAGS := -lstdc++

.PHONY: clean event.cpp checker.cpp

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

utest1: utest1.o event.o
	$(CC) $(LDFLAGS) -o utest1 utest1.o event.o

utest2: utest2.o event.o checker.o
	$(CC) $(LDFLAGS) -o utest2 utest2.o event.o checker.o

utest3: utest3.o event.o checker.o data.o
	$(CC) $(LDFLAGS) -o utest3 utest3.o event.o checker.o data.o

clean_o:
	rm -f *.o

clean: clean_o
	rm -f utest1 utest2 utest3 main
