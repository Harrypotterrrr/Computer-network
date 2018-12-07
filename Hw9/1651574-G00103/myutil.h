#include <iostream>
#include <fstream>
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
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;


#define GETSTUNO 0
#define GETPID 1
#define GETTIME 2
#define GETSTR 3
#define END 4

#define DEAD -1
#define DONE 0
#define ALIVE 1 
#define WAITWRITE 2 



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
	//char * randomMsg ;
	int randomMsgLen ;  //32768-99999

};

struct ClientInfo{
	int cfd ;
	int count ;
	ClientMsg msg;
	int flag  ; 
	int rwLen ;
};


void create_daemon();

void myExit();

void setReusePort(int fd);

void setNonBlock(int fd);

bool writeFile(const ClientInfo & cinfo, bool);

bool testConnect(const int cfd);

void setTime(timeval &timeout, int sec, int usec = 0) ;

int parseCMD(int argc, char **argv ,int isClient );
