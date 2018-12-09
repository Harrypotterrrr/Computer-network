compiler=g++

sendSRCS :=../common/common.cpp sender1.cpp
sendTARGET :=sender3 receiver3 

all:	$(sendTARGET)

sender3 : sender3.cpp ../common/common.cpp
	g++ -o sender3 sender3.cpp ../common/common.cpp -lrt

receiver3 : receiver3.cpp ../common/common.cpp
	g++ -o receiver3 receiver3.cpp ../common/common.cpp -lrt
	cp receiver3 ../test/receiver3

.PHONY:clean
clean:
	rm -rf $(sendTARGET)  network* fifo*
