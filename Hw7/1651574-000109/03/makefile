SRC = server1 client1 server2 client2
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server3-1 tcp_client3-1 tcp_server3-2 tcp_client3-2

main: $(TARGET)

tcp_server3-1: server1.o
	g++ -o $@ $^

tcp_client3-1: client1.o
	g++ -o $@ $^

tcp_server3-2: server2.o
	g++ -o $@ $^

tcp_client3-2: client2.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
