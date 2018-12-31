CC = g++
LIB := ../common/common.cpp

main: sender receiver

sender: sender.cpp $(LIB)
	$(CC) -o $@ $^
	
receiver : receiver.cpp $(LIB)
	$(CC) -o $@ $^

.PHONY : clean
clean :
	rm -f sender receiver *.dat
