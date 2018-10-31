SRC = server1 client1-1 client1-2 server2 client2-1 client2-2
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server4-1 tcp_client4-1-1 tcp_client4-1-2 tcp_server4-2 tcp_client4-2-1 tcp_client4-2-2

main: $(TARGET)

tcp_server4-1: server1.o
	g++ -o $@ $^

tcp_client4-1-1: client1-1.o
	g++ -o $@ $^

tcp_client4-1-2: client1-2.o
	g++ -o $@ $^

tcp_server4-2: server2.o
	g++ -o $@ $^

tcp_client4-2-1: client2-1.o
	g++ -o $@ $^

tcp_client4-2-2: client2-2.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
