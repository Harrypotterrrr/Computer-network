CC = gcc
SUFFIX = .c
CFLAGS = -c

test: libtest1.so
	$(CC) test2$(SUFFIX) -L. -ltest1 -o test
	# you can also add the following sentence to fresh cache automatically
	# ldconfig

libtest1.so: test1$(SUFFIX)
	$(CC) -fPIC -shared -o libtest1.so test1$(SUFFIX)

clean:
	rm -f libtest1.so test