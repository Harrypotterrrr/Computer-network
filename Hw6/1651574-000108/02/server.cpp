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
#include <arpa/inet.h>
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
    int server_fd;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    // if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    //     error_messg = strerror(errno);
    //     cerr << error_messg <<endl;
    //     exit(EXIT_FAILURE);
    // }

    int port = atoi(argv[1]);

    struct sockaddr_in serveAddr;
    serveAddr.sin_family = AF_INET;
    //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
    serveAddr.sin_port = htons(port);
    //设置的端口为INADDR_ANY
    serveAddr.sin_addr.s_addr = htonl(INADDR_ANY);    //  host to unsigned long, INADDR_ANY = 0.0.0.0

    if(bind(server_fd,(struct sockaddr *)&serveAddr,sizeof(serveAddr)) == -1)
        myExit();

    if(listen(server_fd, QUEUE) == -1)
        myExit();

    int connect_fd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t len_client_addr = sizeof(client_addr);

    
    if((connect_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len_client_addr)) == -1)
        myExit();
    cout<<"connection succeed!"<<endl;

    // if(getpeername(connect_fd, (struct sockaddr*)&client_addr, &len_client_addr) == -1)
    //     myExit();
    
    // cout<<"client IP:"<<inet_ntoa(c.sin_addr)<<"  port:"<<ntohs(c.sin_port)<<endl;

    cout<<"client IP: "<<inet_ntoa(client_addr.sin_addr)<<"\tport: "<<ntohs(client_addr.sin_port)<<endl;

        
    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        if(recv(connect_fd, buffer, BUFFER_SIZE, 0) == 0)
            break;  // MSG_WAITALL MSG_DONTWAITALL 0 ???
        else{
            cout << buffer << endl;
            send(connect_fd, buffer, BUFFER_SIZE, 0);
        }
    }

    close(connect_fd);
    close(server_fd);
    return 0;
}
