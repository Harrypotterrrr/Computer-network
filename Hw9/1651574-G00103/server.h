#include "myutil.h"

#define MAX_CONNECT   20
#define BUFFER_SIZE (100)
#define WRITE_TIME 1
#define LEN_SEND 10


struct ClientInfo{
	int cfd ;
	int count ;
	ClientMsg msg;
	bool flag  ; 
};

void createNewConnect();

extern ClientInfo clientInfo[MAX_CONNECT];