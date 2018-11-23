#include "myutil.h"

struct sockaddr_in server_addr;
int port;
bool flag_block;
bool flag_fork;
int max_num;


const char *orderMsg[]={
	"StuNo",
	"pid",
	"Time",
	"str",
	"end"
};

int parseCMD(int argc, char **argv ,int isClient )
{
    flag_block = false;
    flag_fork = false;
    port = -1;
    max_num = 100;


    int i ;
    for (i = 1; argv[i]; i++)
    {
        if (strstr(argv[i], "ip"))
        {
            server_addr.sin_addr.s_addr = inet_addr(argv[i + 1]);
            isClient = false;
            i++;
        }

        else if (strstr(argv[i], "port"))
        {
            if(i>=argc-1){
                fprintf(stderr,"please input port !\n");
                exit(-1);
            }
            port = atoi(argv[i + 1]);
            i++;
        }

        else if (strstr(argv[i], "num"))
        {
            max_num = atoi(argv[i + 1]);
            i++;
        }

        else if (strcmp(argv[i] + 2, "block") == 0)
        {
            flag_block = true;
        }

        // else if(strcmp(argv[i] + 2, "nonblock") == 0){

        // }

        else if (strcmp(argv[i] + 2, "fork") == 0)
        {
            flag_fork = true;
        }

        // else if(strcmp(argv[i] + 2, "nofork") == 0){

        // }
    }

    if(port == -1)
    {
        fprintf(stderr,"please input port !\n");
        exit(-1);
    }

    //if (port == -1)
     //   return false;

    if(isClient){
        fprintf(stderr,"please input ip !\n");
        exit(-1);
    }
    server_addr.sin_port = htons(port);
    return true;
}

bool mySndMsg(int cfd , const void * Msg, int MsgLen )
{
    int sndLen = 0 ;
    int sndNum ;
    while(sndLen <MsgLen){
        sndNum = send(cfd,Msg+sndLen,MsgLen,0);
        if(sndNum<=0)
            return false;
        sndLen +=sndNum ;
    }
    return true ; 
}
