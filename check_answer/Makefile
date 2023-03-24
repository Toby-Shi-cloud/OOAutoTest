CC = g++
CFLAGS = -Wall -Werror -std=c++11 -g
LDFLAGS = -lstdc++

.PHONY: clean

main: main.o event.o checker.o
	$(CC) $(LDFLAGS) -o main main.o event.o checker.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

utest1: utest1.o event.o
	$(CC) $(LDFLAGS) -o utest1 utest1.o event.o

utest2: utest2.o event.o checker.o
	$(CC) $(LDFLAGS) -o utest2 utest2.o event.o checker.o

clean:
	rm *.o utest1 utest2 main
