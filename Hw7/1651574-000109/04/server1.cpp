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
#define QUEUE   20
#define BUFFER_SIZE (88)
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

void setReusePort()
{
    int enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        myExit();
}

void setNonBlock(int *fd, int tag)
{
    int flag;
    if((flag = fcntl(*fd, F_GETFL, 0)) == -1)
        myExit();
    if(fcntl(*fd, F_SETFL, flag | O_NONBLOCK) == -1)
        myExit();

    cout << "set " << (tag == 0 ? "server" : "client") << " non-block succeed" << endl;
}

void processData()
{

    const char send_len = 10;
    const char send_buff[send_len] = "123456789";
    char recv_buff[BUFFER_SIZE] = "\0";

    struct timeval wait_time;

    int ctr_recv_byte = 0, ctr_send_byte = 0;


    while(true) {
        
        FD_ZERO(&read_fds);
        FD_SET(connect_fd, &read_fds);
        wait_time.tv_sec = 0;
        wait_time.tv_usec = 500 * 1000;

        switch(select(connect_fd + 1, &read_fds, &write_fds, NULL, &wait_time)){
            case -1: myExit();
            case 0:
                // printf("reach time limit\n");
                break; // go to loop again to wait
            default:

                if(FD_ISSET(connect_fd, &read_fds)){
                    FD_CLR(connect_fd, &read_fds);
                    int tmp_rtn = recv(connect_fd, recv_buff, sizeof(recv_buff), MSG_DONTWAIT);
                    if(tmp_rtn < 1){
                        cout<<"the end of conversation" <<endl;
                        return;
                    }
                    
                    ctr_recv_byte += tmp_rtn;
                    cout << "recieve from client: " << recv_buff << endl;
                    cout << "have recieved "<<ctr_recv_byte << " bytes" << endl;
                }
                
        }
    }
}

int main(int argc,char* argv[])
{
     if(argc != 2){
        cerr << "Not match parameters!" <<endl;
        exit(EXIT_FAILURE);
    }


    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    setReusePort();

    setNonBlock(&server_fd, 0);

    int port = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

    getchar();

    processData();

    close(connect_fd);
    close(server_fd);
    return 0;
}