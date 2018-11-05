SRC = server client1 client2
OBJ = $(addsuffix .o, $(SRC))
TARGET = tcp_server5 tcp_client5-1 tcp_client5-2

main: $(TARGET)

tcp_server5: server.o
	g++ -o $@ $^

tcp_client5-1: client1.o
	g++ -o $@ $^

tcp_client5-2: client2.o
	g++ -o $@ $^

.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
