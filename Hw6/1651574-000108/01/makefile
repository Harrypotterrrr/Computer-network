SRC = server client
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server1 tcp_client1

main: $(TARGET)

tcp_server1: server.o
	g++ -o $@ $^

tcp_client1: client.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
