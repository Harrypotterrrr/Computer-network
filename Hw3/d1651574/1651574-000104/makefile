DIRS = `find . * -type d | grep -v "\."`

main: test

.PHONY = test clean
test:
	for dir in $(DIRS); do \
		make -C $$dir || echo "with no makefile"; \
	done

clean:
	for dir in $(DIRS); do \
		make -C $$dir clean || echo "with no makefile"; \
	done