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
#define MAX_CONNECT   20
#define BUFFER_SIZE (100)
#define WRITE_TIME 1
#define LEN_SEND 10


extern int errno;
char * error_messg;

int server_fd, connect_fd[MAX_CONNECT];
struct sockaddr_in server_addr, client_addr;
fd_set read_fds, write_fds;

int ctr_client = 0;     // the counter of client
bool flag_client[MAX_CONNECT];   // record the valid of client_fd, whether alive or not

char send_buff[MAX_CONNECT][LEN_SEND];   // the buffer of send
char recv_buff[MAX_CONNECT][BUFFER_SIZE];    // the buffer of recv

struct timeval wait_time;   // select wait_time

int ctr_recv_byte[MAX_CONNECT], ctr_send_byte[MAX_CONNECT];   // counter of recv/send byte

int max_fd; // record the maximum of fd


void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg <<endl;
    exit(EXIT_FAILURE);
}

void writeWait(int sig)
{
    FD_ZERO(&write_fds);
    for(int i=0 ; i<ctr_client ; i++){
        if(!flag_client[i])
            continue;
        FD_SET(connect_fd[i], &write_fds);
    }
    alarm(1);
}

void init()
{
    max_fd = server_fd;
    for(int i=0 ; i<MAX_CONNECT ; i++){
        ctr_client = 0;
        flag_client[i] = false;
        strcpy(send_buff[i],"123456789");
        recv_buff[i][0] = '\0';
        ctr_recv_byte[i] = ctr_send_byte[i] = 0;
    }
}

void printServerInfo()
{
    cout << "=============================\n";
    cout << "The socket binds to:"<< endl;
    cout << "IP: " << inet_ntoa(server_addr.sin_addr) << endl;
    cout << "port: " << ntohs(server_addr.sin_port) <<endl;
    cout << "==============================\n";
}

void printClientInfo(int no)
{
    cout << "------------------------------"<<endl;
    cout << "[#No. " << no <<"]: connection succeed!"<<endl;
    cout << "[#No. "<< no<<"]: client IP: "<<inet_ntoa(client_addr.sin_addr)<<"\tport: "<<ntohs(client_addr.sin_port)<<endl;
    cout << "------------------------------"<<endl;
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


void sendMsg()
{
    for(int i=0 ; i < ctr_client ; i++){
        if(!flag_client[i])
            continue;
        if(FD_ISSET(connect_fd[i], &write_fds)){
            FD_CLR(connect_fd[i], &write_fds);
            int tmp_rtn = send(connect_fd[i], send_buff[i], LEN_SEND, MSG_DONTWAIT);
            if(tmp_rtn < 1){
                cout<<"[No. " << i <<"]: the end of conversation" <<endl;
                flag_client[i] = false;
                close(connect_fd[i]);
            }

            ctr_send_byte[i] += tmp_rtn;
            cout<<"[No. " << i <<"]: send to client: " << send_buff[i] << endl;
            cout<<"[No. " << i <<"]: "<< ctr_send_byte[i] << " bytes have sent " << endl;
        }
    }
}

void recvMsg()
{
    for(int i=0 ; i < ctr_client ; i++){
        if(!flag_client[i])
            continue;
        if(FD_ISSET(connect_fd[i], &read_fds)){
            FD_CLR(connect_fd[i], &read_fds);
            int tmp_rtn = recv(connect_fd[i], recv_buff[i], sizeof(recv_buff[i]), MSG_DONTWAIT);
            if(tmp_rtn < 1){
                cout << "------------------------------"<<endl;
                cout<<"[No. " << i <<"]: the end of conversation" <<endl;
                cout << "------------------------------"<<endl;
                flag_client[i] = false;
                close(connect_fd[i]);
            }
            
            ctr_recv_byte[i] += tmp_rtn;
            cout<<"[No. " << i <<"]: recieve from client: " << recv_buff[i] << endl;
            cout<<"[No. " << i <<"]: "<< ctr_recv_byte[i] << " bytes have recieved " << endl;
        }
    }
}

void createNewConnect()
{
    socklen_t len_client_addr = sizeof(client_addr);
    int i = ctr_client++;
    if((connect_fd[i] = accept(server_fd, (struct sockaddr*)&client_addr, &len_client_addr)) == -1)
        myExit();
    
    max_fd = max_fd > connect_fd[i] ? max_fd : connect_fd[i];

    printClientInfo(i);

    setNonBlock(&connect_fd[i], 1);

    flag_client[i] = true;
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

    if(listen(server_fd, MAX_CONNECT) == -1)
        myExit();
    
    cout << "server is listening" <<endl;
    

    init();

    signal(SIGALRM, writeWait);
    alarm(WRITE_TIME);
    while(true){
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        for(int i=0 ; i<ctr_client ; i++){
            if(!flag_client[i])
                continue;
            FD_SET(connect_fd[i], &read_fds);
        }

        // wait_time.sec = 3;
        // wait_time.usec = 0;

        switch(select(max_fd + 1, &read_fds, &write_fds, NULL, NULL)){
            case -1:    // alarm ring to arouse the -1 return of select function
                if(errno != EINTR)
                    myExit();
                sendMsg();
            case 0: break;
            default:
                if(FD_ISSET(server_fd, &read_fds))
                    createNewConnect();
                recvMsg();
        }
    }

    close(server_fd);
    return 0;
}