SUBDIR = $(shell ls -F | grep /$)
#SUBDIR := $(shell grep '*/' $(SUBDIR))
main:
	@for dir in $(SUBDIR); \
	do (make -C $$dir); \
	done

.PHONY= clean

clean:
	@for dir in $(SUBDIR); \
	do (make -C $$dir  clean); \
	done
