compiler=g++

sendSRCS :=../common/common.cpp sender1.cpp
sendTARGET :=sender2 receiver2 

all:	$(sendTARGET)

sender2 : sender2.cpp ../common/common.cpp
	g++ -o sender2 sender2.cpp ../common/common.cpp -lrt

receiver2 : receiver2.cpp ../common/common.cpp
	g++ -o receiver2 receiver2.cpp ../common/common.cpp -lrt
	cp receiver2 ../test/receiver2

.PHONY:clean
clean:
	rm -rf $(sendTARGET)  network* fifo*
