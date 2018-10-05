CC = g++
FLAGS = -c
objects = test1.o test2.o test3.o
main: $(objects)
	g++ -o main $(objects)
# use implicit rules

.PHONY: clean
clean:
	rm -f $(objects) main

