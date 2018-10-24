SRC = test SubProcess
OBJ = $(addsuffix .o,$(SRC))
TARGET = test

CC = g++
CFLAGS_LIB := -shared -fPIC
CFLAGS_LIB_SELFPATH := -Wl,-rpath=.
CFLAGS_v = -std=c++14

ifeq ($(PREFIX),)
	PREFIX := /usr
endif

main: $(TARGET)
# main: $(TARGET)_local

$(TARGET): $(OBJ) lib1651574.so test.h 
	$(CC) -L. -l1651574 -o $@ $(OBJ)

$(TARGET)_local: $(OBJ) lib1651574.so test.h 	# enable to run under current direcory
	$(CC) -L. -l1651574 $(CFLAGS_LIB_SELFPATH) -o $@ $(OBJ)

SubProcess.o: SubProcess.cpp test.h
	$(CC) -c SubProcess.cpp -o SubProcess.o

test.o: test.cpp test.h
	$(CC) -c test.cpp -o test.o

lib1651574.so : readconfig.cpp
	$(CC) $(CFLAGS_LIB) -o $@ $^



.PHONY: clean
clean:
	$(RM) *.o $(TARGET) lib1651574.so $(TARGET)_local *.rpm

.PHONY : rpm
rpm : clean
	mkdir -p ~/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
	yes | cp -rf ./* ~/rpmbuild/BUILD/	# with yes pip, to send `yes` response to the `cp -f` enforcing instruction
	rpmbuild -ba ./test-1651574.spec
	yes | cp -f ~/rpmbuild/RPMS/x86_64/test-1651574-1.0.0-1.el7.centos.x86_64.rpm ./test-1651574.rpm


# if the program don't specify the $(DESTDIR), which means `make` instruction is not from the *.spec file, the value of $(DESTDIR) will be NULL
.PHONY: install
install: $(TARGET) 
	install -d $(DESTDIR)$(PREFIX)/lib64
	install ./lib1651574.so $(DESTDIR)$(PREFIX)/lib64/

	install -d $(DESTDIR)$(PREFIX)/sbin
	install ./test $(DESTDIR)$(PREFIX)/sbin/test-1651574

	install -d $(DESTDIR)$(PREFIX)/1651574
	install ./1651574.dat $(DESTDIR)$(PREFIX)/1651574/

	install -d $(DESTDIR)/etc
	install ./1651574.conf $(DESTDIR)/etc/

	install -d $(DESTDIR)$(PREFIX)/lib/systemd/system

	install ./test-1651574.service $(DESTDIR)$(PREFIX)/lib/systemd/system/


.PHONY: uninstall
uninstall: clean
	$(RM) $(DESTDIR)$(PREFIX)/lib64/lib1651574.so

	$(RM) $(DESTDIR)$(PREFIX)/sbin/test-1651574 

	$(RM) $(DESTDIR)$(PREFIX)/1651574/1651574.dat 

	$(RM) $(DESTDIR)/etc/1651574.conf

	$(RM) $(DESTDIR)$(PREFIX)/lib/systemd/system/test-1651574.service

	$(RM) ~/rpmbuild/RPMS/x86_64/test-1651574-1.0.0-1.el7.centos.x86_64.rpm
	$(RM) ~/rpmbuild/SRPMS/test-1651574-1.0.0-1.el7.centos.src.rpm
	$(RM) ~/rpmbuild/BUILD/*

	# conditional delete
	if [ "`ls -A /usr/1651574`" = "" ]; then `rm -rf /usr/1651574`;fi;