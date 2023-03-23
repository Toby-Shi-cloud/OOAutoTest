CC = g++
CFLAGS = -Wall -Werror
LDFLAGS = 

.PHONY: clean

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

utest1: utest1.o event.o
	$(CC) $(LDFLAGS) -o utest1 utest1.o event.o

clean:
	rm *.o
	rm utest1
