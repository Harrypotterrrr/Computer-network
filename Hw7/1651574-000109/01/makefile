SRC = server1 client1 server2 client2 server3 client3 
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server1-1 tcp_client1-1 tcp_server1-2 tcp_client1-2 tcp_server1-3 tcp_client1-3 

main: $(TARGET)

tcp_server1-1: server1.o
	g++ -o $@ $^

tcp_client1-1: client1.o
	g++ -o $@ $^

tcp_server1-2: server2.o
	g++ -o $@ $^

tcp_client1-2: client2.o
	g++ -o $@ $^

tcp_server1-3: server3.o
	g++ -o $@ $^

tcp_client1-3: client3.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
