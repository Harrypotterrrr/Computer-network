#include <iostream>
#include <fstream>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/file.h>//flock,lockf
#include <errno.h>
#include <signal.h>
#include <sys/prctl.h>//修改进程名
#include <queue>

using namespace std; 

#define MAX_PKT 1024 //数据
#define FALSE 0
#define ERROR -1
#define OK 1

#define BUFSIZE 1024 
#define DATASIZE 1024*1024 
#define PATHLENGTH 40
#define FRAMESIZE 1024+12


#define FIFO_TO_NETWORK "./fifo_to_network.file"
#define FIFO_TO_PHYSICAL "./fifo_to_physical.file"
#define FIFO_NT_TO_DT "./fifo_nt_to_dt.file"
#define FIFO_PH_TO_DT "./fifo_ph_to_dt.file"

#define SIG_CHSUM_ERR	35
#define SIG_FRAME_ARRIVAL 36
#define SIG_NETWORK_LAYER_READY 37
#define SIG_ENABLE_NETWORK_LAYER 38
#define SIG_DISABEL_NETWORK_LAYER 39

#define SIG_DTLINK_LAYER_READY 40 

#define SIG_PL_CONNECT 41

#define SIG_DONE 43

//#define SIG_NETWORK_FROM_DATALK  45
#define SIG_NETWORK_TO_DATALK  46

#define SIG_DATALK_FROM_NETWORK 47
#define SIG_DATALK_TO_PHYSIC  50
/* #define SIG_DATALK_TO_NETWORK  48
#define SIG_DATALK_FROM_PHYSIC  49
#define SIG_DATALK_TO_PHYSIC  50

#define SIG_PHYSIC_FROM_DATALK  51
#define SIG_PHYSIC_TO_DATALK  52 */


typedef int Status;
typedef enum {frame_arrival=1,chsum_err,timeout,ack_timeout,network_layer_ready} event_type;
typedef enum {data,ack,nak} frame_kind;
typedef unsigned int seq_nr; 

struct Packet
{
	unsigned char data[MAX_PKT];
};

struct Frame
{
	int kind; 
	seq_nr seq;
	seq_nr ack;
	Packet info;
};

extern bool PL_Connect ;

// Net 向 datalink 管道中的包数目
extern int nNet_to_dl ;
extern int nDl_to_phy;
extern int nPhy_to_dl;
extern int nDl_to_net;

extern int fifo_dtop;
extern int fifo_ptod;
extern int fifo_dton;

extern pid_t nt_pid; // 网络层
extern pid_t dl_pid; // 数据链路层
extern pid_t ps_pid; // 物理层

extern queue<event_type> eventQueue ; 

Status datalink_from_network(Packet& buffer, int k);

Status datalink_to_physical(Frame& s);

Status datalink_from_physical(Frame& r);

Status datalink_to_network_layer(Packet& buffer);

Status physical_layer_from_datalink(Frame &r);

Status network_to_datalink(Packet& buffer, int k);

Status network_from_datalink (Packet &buffer );

Status physical_to_datalink(Frame &r );

Status physical_from_datalink(Frame &r);


void enable_network_layer();

void disable_network_layer();

void sig_catch();

Status lock_set(int fd, int type) ;

 void generateData(char buf[]);

 void getTestPath(int k,char path[]);

void wait_for_event(event_type* event);

 void create_daemon() ;

 void myExit();

 void Init();

// 获取所有 层级的pid 
 int getLayerPid(int & net , int & dt , int & ps ,const char * filename);

 bool allZero(const Packet & p);


//在 进入数据传递前需要调用这个函数，将所有层的fifo建立
void connectFifo();

// 
void wait_for_event( event_type &myevent);

// 获取数据帧的长度
int frameSize( const Frame & s ) ;

void setNonBlock(int fd);

void setTime(timeval &timeout, int sec, int usec );