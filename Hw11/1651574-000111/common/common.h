#pragma once
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/param.h>
#include <netinet/in.h> 
#include <fcntl.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/shm.h>
#include <time.h>

using namespace std;

extern const int ERR_SIGNAL;
extern const int MYSIGNAL;
extern int l1_datalen;
extern int l2_srcport;
extern int l2_dstport;
extern int l2_offset;
extern int l2_flag;
extern int l3_flag;
extern int l3_offset;
extern int l3_ttl;
extern int l3_srcip;
extern int l3_dstip;
extern int l4_srcmac[6];
extern int l4_dstmac[6];
extern int sig_flag;
extern const char* SENDER_DAT;
extern const char* NETWORK_DAT;
extern const char * RECEIVER_DAT;
void createDaemon();
void readConfig();
short tcpCheck(char *tcp_arr,int source_addr,int dest_addr,short len);
short ipCheck(char *ip_arr);

int randInt();
short randShort();
char randChar();
void outputFile(const char*,int,const char*);
int inputFile(char *p,const char* filename);
void initFile(char *p);

void sndSolveTcp(char *p);
void sndSolveIp(char *p);
void sndSolveDl(char *p);

void initSig();

void printApp(const char *,int);
void printTcp(const char *);
void printIp(const char *);
void printDl(const char *);
//void show_phy(const char *);
