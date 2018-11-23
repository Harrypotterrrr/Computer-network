SRC = server client
OBJ = $(addsuffix .o, $(SRC))
TARGET = server client

main: $(TARGET)

server: server.o
	g++ -o $@ $^

client: client.o
	g++ -o $@ $^


.PHONY: clean

clean:
	$(RM) $(TARGET) $(OBJ)
