compiler=g++

sendSRCS :=../common/common.cpp sender1.cpp
sendTARGET :=sender1 receiver1 

all:	$(sendTARGET)

sender1 : sender1.cpp ../common/common.cpp
	g++ -lrt -o sender1 sender1.cpp ../common/common.cpp

receiver1 : receiver1.cpp ../common/common.cpp
	g++ -lrt -o receiver1 receiver1.cpp ../common/common.cpp
	cp receiver1 ../test/receiver1

.PHONY:clean
clean:
	rm -rf $(sendTARGET) 1651574* network* fifo*
