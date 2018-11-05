DIRS = `find . * -type d | grep -v "\."`

all: main

main:
	for dir in $(DIRS); do \
		make -C $$dir || echo "with no makefile"; \
	done
	
.PHONY = test clean

clean:
	for dir in $(DIRS); do \
		make -C $$dir clean || echo "with no makefile"; \
	done