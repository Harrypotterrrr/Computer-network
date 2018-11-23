#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <fcntl.h>
#include <net/if.h>
#include <signal.h>
#include <netdb.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;


#define GETSTUNO 0
#define GETPID 1
#define GETIME 2
#define GETSTR 3
#define END 4

extern int errno;

extern struct sockaddr_in server_addr;
extern int port;
extern bool flag_block;
extern bool flag_fork;
extern int max_num;
extern const char * orderMsg [];


struct ClientMsg{

	int StuNo;
	int clientPid ;
	char clientTime [20];
	char randomMsg [100000];
	int randomMsgLen ;  //32768-99999

};



// bool sSnd(int count , int cfd );

// bool sRecv(int  count , int cfd , struct ClientMsg * cMsg);


int parseCMD(int argc, char **argv ,int isClient );


bool mySndMsg(int cfd ,const  void * Msg, int MsgLen );
