SRC = server1 client1 server2 client2 server3 client3 server4 client4
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server6-1 tcp_client6-1 tcp_server6-2 tcp_client6-2 tcp_server6-3 tcp_client6-3 tcp_server6-4 tcp_client6-4

main: $(TARGET)

tcp_server6-1: server1.o
	g++ -o $@ $^

tcp_client6-1: client1.o
	g++ -o $@ $^

tcp_server6-2: server2.o
	g++ -o $@ $^

tcp_client6-2: client2.o
	g++ -o $@ $^

tcp_server6-3: server3.o
	g++ -o $@ $^

tcp_client6-3: client3.o
	g++ -o $@ $^

tcp_server6-4: server4.o
	g++ -o $@ $^

tcp_client6-4: client4.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
