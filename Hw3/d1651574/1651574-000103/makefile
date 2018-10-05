CC = g++
CFLAGS = -c
DIRS = $(shell find . * -type d | grep -v "\.")

.PHONY = clean debug test

main: test

test:
	for dir in $(DIRS);do \
		make -C $$dir || echo "with no makefile";\
	done

clean:
	for dir in $(DIRS);do \
		make -C $$dir clean || echo "with no makefile"; \
	done

debug:
	@echo `find . * -type d`
	@echo `find . -type d`
	@echo `find . * -type d | grep -v "\."`
	@echo `find . -type d | grep -v "\."`