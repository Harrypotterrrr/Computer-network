SRC = server client
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server2 tcp_client2

main: $(TARGET)

tcp_server2: server.o
	g++ -o $@ $^

tcp_client2: client.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
