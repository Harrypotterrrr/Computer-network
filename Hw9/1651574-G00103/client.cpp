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
#define BUFFER_SIZE (100)
#define MAX_CONNECT 2
#define WRITE_TIME 3

extern int errno;
char * error_messg;

int client_fd[MAX_CONNECT];
fd_set read_fds, write_fds;

int max_fd;

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

void writeWait(int sig)
{
    FD_ZERO(&write_fds);
    for(int i=0 ; i<MAX_CONNECT ; i++)
        FD_SET(client_fd[i], &write_fds);
    alarm(3);
}

void processData()
{
    const int send_len = 15;
    const char send_buff[send_len] = "abcd9876543210";
    
    char recv_buff[BUFFER_SIZE] = "\0";

    int ctr_recv_byte = 0, ctr_send_byte = 0;

    struct timeval wait_time;

    signal(SIGALRM, writeWait);
    alarm(WRITE_TIME);
    while (true) {

        FD_ZERO(&read_fds);
        for(int i=0 ; i<MAX_CONNECT ; i++)
            FD_SET(client_fd[i], &read_fds);
        wait_time.tv_sec = 3; 
        wait_time.tv_usec = 0;

        switch(select(max_fd + 1, &read_fds, &write_fds, NULL, &wait_time)){
            case -1: 
                if(errno != EINTR)
                    myExit();

                for(int i=0 ; i<MAX_CONNECT ; i++)
                    if(FD_ISSET(client_fd[i], &write_fds)){
                        FD_CLR(client_fd[i], &write_fds);
                        int tmp_rtn = send(client_fd[i], send_buff, send_len, MSG_DONTWAIT);
                        if(tmp_rtn < 1)
                            myExit();

                        ctr_send_byte += tmp_rtn;
                       cout<<"[No. " << i <<"]: send to server: " << send_buff << endl;
                        cout << ctr_send_byte << " bytes have sent " << endl;
                    }
                
            case 0: 
                // printf("reach time limit\n");
                break; // go to loop again to wait
            default:

                for(int i=0 ; i<MAX_CONNECT ; i++)
                    if(FD_ISSET(client_fd[i], &read_fds)){
                        FD_CLR(client_fd[i], &read_fds);
                        int tmp_rtn = recv(client_fd[i], recv_buff, sizeof(recv_buff), MSG_DONTWAIT);
                        if(tmp_rtn < 1)
                            break;
                            
                        ctr_recv_byte += tmp_rtn;
                        cout<<"[No. " << i <<"]: recieve from server: " << recv_buff << endl;
                        cout << ctr_recv_byte << " bytes have recieved " << endl;
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
     if(argc != 4){
        cerr << "Not match parameters!" <<endl;
        exit(EXIT_FAILURE);
    }

    // signal(SIGINT, interuptRes);

    max_fd = 0;
    for(int i=0 ; i < MAX_CONNECT ; i++){
        if((client_fd[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            myExit();
        max_fd = max_fd > client_fd[i] ? max_fd : client_fd[i];
    }

    // int port_client = atoi(argv[1]);
    char* ip_server[MAX_CONNECT];
    int port_server[MAX_CONNECT];

    ip_server[0] = argv[1];
    port_server[0] = atoi(argv[2]);
    port_server[1] = atoi(argv[3]);

    
    for(int i=0 ; i<MAX_CONNECT ; i++)
        setNonBlock(&client_fd[i]);


    struct sockaddr_in server_addr[MAX_CONNECT];
    for(int i=0 ; i < MAX_CONNECT ; i++){
        server_addr[i].sin_family = AF_INET;
        server_addr[i].sin_port = htons(port_server[i]);
        server_addr[i].sin_addr.s_addr = inet_addr(ip_server[0]);
    }

    struct sockaddr_in client_addr;
    // client_addr.sin_port = htons(port_client);
    // use default, not change
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    // client_addr.sin_addr.s_addr = inet_addr("192.168.60.231");

    for(int i=0 ; i< MAX_CONNECT ; i++){
        // if(bind(client_fd[i],(struct sockaddr *)&client_addr,sizeof(client_addr)) == -1)
        //     myExit();
        if(connect(client_fd[i], (struct sockaddr *)&server_addr[i], sizeof(server_addr[i])) == -1)
            if( errno != EINPROGRESS )  
                myExit();
        
        cout << "------------------------------"<<endl;
        cout << "[#No. " << i <<"]: connection succeed!"<<endl;
        cout << "[#No. "<< i<<"]: server IP: "<<inet_ntoa(server_addr[i].sin_addr)<<"\tport: "<<ntohs(server_addr[i].sin_port)<<endl;
        cout << "------------------------------"<<endl;
    }

    processData();

    for(int i=0 ; i<MAX_CONNECT ; i++)
        close(client_fd[i]);
    
    return 0;
}
