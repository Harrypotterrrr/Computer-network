compiler=g++

sendSRCS :=../common/common.cpp sender4.cpp
sendTARGET :=sender4 receiver4 

all:	$(sendTARGET)

sender4 : sender4.cpp ../common/common.cpp
	g++ -o sender4 sender4.cpp ../common/common.cpp -lrt

receiver4 : receiver4.cpp ../common/common.cpp
	g++ -o receiver4 receiver4.cpp ../common/common.cpp -lrt

.PHONY:clean
clean:
	rm -rf $(sendTARGET) 1651574* network* fifo*
