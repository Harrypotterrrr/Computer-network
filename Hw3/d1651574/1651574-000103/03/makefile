CC = g++
CFLAGS = -c
EXE = test1 test2 test3

test: $(EXE)

test1: test1.o
	$(CC) -o test1 test1.o
test2: test2.o
	$(CC) -o test2 test2.o
test3: test3.o
	$(CC) -o test3 test3.o

.PHONY: clean
clean: cleanexe cleano
cleanexe:
	rm -f $(EXE)
cleano:
	rm -f *.o