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
#include <netdb.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;
#define BUFFER_SIZE (100)

extern int errno;
char * error_messg;

int client_fd;
fd_set read_fds, write_fds;

// int ctr = 0;

// void interuptRes(int sig)
// {
//     cout << "The Total counter reaches: " << ctr << endl;
// }

void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg <<endl;
    exit(EXIT_FAILURE);
}


void processData()
{
    const int send_len = 100;
    const char send_buff[send_len] = "THIS IS A PART OF THE SEND MESSAGE!";
    
    int ctr_recv_byte = 0, ctr_send_byte = 0;

    struct timeval wait_time;

    while (true) {

        FD_ZERO(&write_fds);
        FD_SET(client_fd, &write_fds);

        wait_time.tv_sec = 0; 
        wait_time.tv_usec = 500 * 1000;


        switch(select(client_fd + 1, NULL, &write_fds, NULL, &wait_time)){
            case -1: myExit();
            case 0: 
                // printf("reach time limit\n");
                break; // go to loop again to wait
            default:
                if(FD_ISSET(client_fd, &write_fds)){
                    FD_CLR(client_fd, &write_fds);
                    int tmp_rtn = send(client_fd, send_buff, send_len, MSG_DONTWAIT);
                    if(tmp_rtn == -1)
                        myExit();

                    ctr_send_byte += tmp_rtn;
                    cout << "send to server: " << send_buff << endl;
                    cout << "have sent "<<ctr_send_byte << " bytes" << endl;
                }
        }
    }
}

void setNonBlock(int *fd)
{
    int flag;
    if((flag = fcntl(*fd, F_GETFL, 0)) == -1)
        myExit();
    if(fcntl(*fd, F_SETFL, flag | O_NONBLOCK) == -1)
        myExit();

    cout << "set non-block succeed" << endl;
}

int main(int argc,char* argv[])
{
     if(argc != 3){
        cerr << "Not match parameters!" <<endl;
        exit(EXIT_FAILURE);
    }

    // signal(SIGINT, interuptRes);

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    // int port_client = atoi(argv[1]);
    char* ip_server = argv[1];
    int port_server = atoi(argv[2]);

    
    setNonBlock(&client_fd);


    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_server);
    server_addr.sin_addr.s_addr = inet_addr(ip_server);

    struct sockaddr_in client_addr;
    // client_addr.sin_port = htons(port_client);
    // use default, not change
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // client_addr.sin_addr.s_addr = inet_addr("192.168.60.231");

    if(bind(client_fd,(struct sockaddr *)&client_addr,sizeof(client_addr)) == -1)
        myExit();

    if(connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        if( errno != EINPROGRESS )  
            myExit();
    }
    // the connection is still going on!


    processData();

    close(client_fd);
    return 0;
}
