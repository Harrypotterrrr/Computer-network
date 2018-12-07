#include <iostream>
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
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/file.h>//flock,lockf
#include <errno.h>
#include <signal.h>
#include <sys/prctl.h>//修改进程名

using namespace std; 

#define MAX_PKT 1024 //数据
#define FALSE 0
#define ERROR -1
#define OK 1

#define BUFSIZE 1024 
#define DATASIZE 1024*1024 
#define PATHLENGTH 40
#define FRAMESIZE 1024+12

#define FIFO_TO_NETWORK "fifo_to_network.file"
#define FIFO_TO_PHYSICAL "fifo_to_physical.file"
#define FIFO_TO_DATALINK "fifo_to_datalink.file"

#define SIG_CHSUM_ERR	35
#define SIG_FRAME_ARRIVAL 36
#define SIG_NETWORK_LAYER_READY 37
#define SIG_ENABLE_NETWORK_LAYER 38
#define SIG_DISABEL_NETWORK_LAYER 39

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
	frame_kind kind;	
	seq_nr seq;
	seq_nr ack;
	Packet info;
};


Status from_network_layer(Packet* buffer,char sharedFilePath[]);

Status to_physical_layer(Frame* s);

Status from_physical_layer(Frame* r);

Status to_network_layer(Packet* buffer);

Status physical_layer_from_datalink(Frame *r);

void enable_network_layer();

void disable_network_layer();

void getSharedFilePath(int k,char path[]);

Status lock_set(int fd, int type) ;

 void generateData(char buf[]);

 void getTestPath(int k,char path[]);