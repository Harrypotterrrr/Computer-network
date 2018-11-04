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
#define BUFFER_SIZE (1024 * 4)
extern int errno;
char * error_messg;

void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg <<endl;
    exit(EXIT_FAILURE);
}

int main(int argc,char* argv[])
{
     if(argc != 2){
        cerr << "Not match parameters!" <<endl;
        exit(EXIT_FAILURE);
    }

    int server_fd, connect_fd;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    int enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        myExit();


    int flag;
    if((flag = fcntl(connect_fd, F_GETFL, 0)) == -1)
        myExit();
    if(fcntl(connect_fd, F_SETFL, flag | O_NONBLOCK) == -1)
        myExit();

    cout<<"set non-block succeed" <<endl;


    int port = atoi(argv[1]);
    // int readByte = atoi(argv[2]);
    // int writeByte = atoi(argv[3]);
    // if(readByte > BUFFER_SIZE || writeByte > BUFFER_SIZE){
    //     cerr << "write or read byte per time is too large!" <<endl;
    //     exit(EXIT_FAILURE);
    // }

    struct sockaddr_in serveAddr;
    serveAddr.sin_family = AF_INET;
    serveAddr.sin_port = htons(port);
    serveAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_fd,(struct sockaddr *)&serveAddr,sizeof(serveAddr)) == -1)
        myExit();

    cout << "----------------------------------\n";
    cout << "The socket binds to:"<< endl;
    cout << "IP: " << inet_ntoa(serveAddr.sin_addr) << endl;
    cout << "port: " << ntohs(serveAddr.sin_port) <<endl;
    cout << "----------------------------------\n";

    if(listen(server_fd, QUEUE) == -1)
        myExit();

    struct sockaddr_in client_addr;
    socklen_t len_client_addr = sizeof(client_addr);
    
    if((connect_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len_client_addr)) == -1)
        myExit();

    cout<<"connection succeed!"<<endl;

    // if(getpeername(connect_fd, (struct sockaddr*)&client_addr, &len_client_addr) == -1)
    //     myExit();
    
    // cout<<"client IP:"<<inet_ntoa(c.sin_addr)<<"  port:"<<ntohs(c.sin_port)<<endl;

    cout<<"client IP: "<<inet_ntoa(client_addr.sin_addr)<<"\tport: "<<ntohs(client_addr.sin_port)<<endl;


    // set non block

    char buffer[BUFFER_SIZE];
    struct timeval wait_time;
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(connect_fd, &fds);
    while(true) {

        wait_time.tv_sec = 3; 
        wait_time.tv_usec = 0;

        memset(buffer, 0, BUFFER_SIZE);
        int recv_rtn;
        switch(select(connect_fd+1, &fds, NULL, NULL, &wait_time)){
            case -1:
                myExit();
            case 0:
                cout<< "reach time limit" << endl; break;
            default:
                recv_rtn = recv(connect_fd, buffer, BUFFER_SIZE, MSG_DONTWAIT);
                if(recv_rtn == -1){
                    cout<< "return is -1, ERROR" << endl;
                    break; 
                }
                else if(recv_rtn == 0){
                    cout<< "return is 0, recieve NONE" << endl;
                    break; 
                }
                cout << "recieve str: "<< buffer << endl;
                send(connect_fd, buffer, BUFFER_SIZE, MSG_DONTWAIT);
        }

        if(recv_rtn <= 0)
            break;
    }

    close(connect_fd);
    close(server_fd);
    return 0;
}
