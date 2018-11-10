SRC = test2-1 test2-2 test2-3 test2-4-1 test2-4-2 test2-5-1 test2-5-2
OBJ = $(addsuffix .o, $(SRC))
TAR = $(SRC)

all: $(TAR)

.PHONY: clean

clean:
	$(RM) $(OBJ) $(TAR)