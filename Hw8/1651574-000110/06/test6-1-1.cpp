#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/shm.h>
#include <net/if.h>
#include <netdb.h>
#include <sys/ioctl.h>



using namespace std;
#define QUEUE   20
#define BUFFER_SIZE (1024)
extern int errno;
char * error_messg;

int server_fd, connect_fd;
struct sockaddr_in server_addr, client_addr;
fd_set read_fds, write_fds;



void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg <<endl;
    exit(EXIT_FAILURE);
}


void printServerInfo()
{
    cout << "----------------------------------\n";
    cout << "The socket binds to:"<< endl;
    cout << "IP: " << inet_ntoa(server_addr.sin_addr) << endl;
    cout << "port: " << ntohs(server_addr.sin_port) <<endl;
    cout << "----------------------------------\n";
}

void printClientInfo()
{
    cout<<"client IP: "<<inet_ntoa(client_addr.sin_addr)<<"\tport: "<<ntohs(client_addr.sin_port)<<endl;
}

void processData()
{
    const int sendLength = 10;

    char sendbuff[sendLength] = "\0";
    char recvbuff[BUFFER_SIZE] = "\0";
    
    const char tmp_str[] = "123456789";
    strcpy(sendbuff, tmp_str);

    int ctr_recv_byte = 0, ctr_send_byte = 0;

    bool flag = false;

    struct timeval wait_time;

    while (true) {

        if(!flag){
            FD_ZERO(&write_fds);
            FD_SET(connect_fd, &write_fds);
        }
        else{
            FD_ZERO(&read_fds);
            FD_SET(connect_fd, &read_fds);
        }
        wait_time.tv_sec = 3; 
        wait_time.tv_usec = 0;

        switch(select(connect_fd + 1, &read_fds, &write_fds, NULL, &wait_time)){
            case -1: printf("-1\n"); myExit();
            case 0: 
                printf("reach time limit\n");
                break; // go to loop again to wait
            default:
                if(FD_ISSET(connect_fd, &write_fds)){
                    FD_CLR(connect_fd, &write_fds);
                    cout << "send to server: " << sendbuff << endl;
                    int tmp_rtn = send(connect_fd, sendbuff, sendLength, MSG_DONTWAIT);
                    if(tmp_rtn < 1)
                        myExit();
                    ctr_send_byte += tmp_rtn;
                    cout << "have sent "<<ctr_send_byte << " bytes" << endl;
                    flag = true;
                }
                if(FD_ISSET(connect_fd, &read_fds)){
                    FD_CLR(connect_fd, &read_fds);
                    int tmp_recv = recv(connect_fd, recvbuff, sizeof(recvbuff), MSG_DONTWAIT);
                    if(tmp_recv == -1)
                        break;
                    ctr_recv_byte += tmp_recv;
                    cout << "recieve from server: " << recvbuff << endl;
                    cout << "have recieved " << ctr_recv_byte << " bytes" << endl;
                    flag = false;
                }
                sleep(1);

        }
    }
}


int main()
{
    int socketID;
    if((socketID = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        myExit();

    if(bind(server_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == -1)
        myExit();

    printServerInfo();

    if(listen(server_fd, QUEUE) == -1)
        myExit();
    
    
    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);
    if(select(server_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
        myExit();


    socklen_t len_client_addr = sizeof(client_addr);
    if((connect_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len_client_addr)) == -1)
        myExit();

    cout<<"connection succeed!"<<endl;

    printClientInfo();

    setNonBlock(&connect_fd, 1);

    processData();

    close(connect_fd);
    close(server_fd);
    return 0;
}
