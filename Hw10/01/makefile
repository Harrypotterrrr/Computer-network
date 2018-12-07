compiler=g++

sendSRCS :=../common/common.cpp sender1.cpp
sendTARGET :=sender1

all:
	$(compiler) -o $(sendTARGET) $(sendSRCS)

.PHONY:clean
clean:
	rm -rf $(sendTARGET) 1651574* network* fifo*
