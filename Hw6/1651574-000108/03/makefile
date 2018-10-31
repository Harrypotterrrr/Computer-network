SRC = server client
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server3 tcp_client3

main: $(TARGET)

tcp_server3: server.o
	g++ -o $@ $^

tcp_client3: client.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
