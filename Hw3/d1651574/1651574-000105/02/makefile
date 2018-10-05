CC = g++
SUFFIX = .cpp
CFLAGS = -c

test: libtest1.so
	$(CC) test2$(SUFFIX) -L. -ltest1 -o test

libtest1.so: test1$(SUFFIX)
	$(CC) -fPIC -shared -o libtest1.so test1$(SUFFIX)

clean:
	rm -f libtest1.so test