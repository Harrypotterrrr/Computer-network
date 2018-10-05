CC = g++
CFLAGS = -c
objects = test1.o test2.o test3.o

main: $(objects)
	g++ -o main $(objects)

test1.o: test.h

test2.o: test.h

test3.o: test.h

.PHONY: clean

clean:
	rm -f *.o main