#include <iostream>
#include <sstream>
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
#define BUFFER_SIZE (1024 * 4)

int main(int argc,char* argv[])
{
    ///定义sockfd
    int client_fd;

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        cerr << "create socket error!"<<endl;
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);

    ///定义sockaddr_in
    struct sockaddr_in serveAddr;
    serveAddr.sin_family = AF_INET;
    //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
    serveAddr.sin_port = htons(port);
    //设置的端口为INADDR_ANY
    serveAddr.sin_addr.s_addr = inet_addr(argv[1]);
    // 服务器ip

    if(connect(client_fd, (struct sockaddr *)&serveAddr, sizeof(serveAddr)) == -1){
        cerr << "connect to server error!"<<endl;
        exit(EXIT_FAILURE);
    }
    else
        cout<<"connection succeed!"<<endl;

    char sendbuff[BUFFER_SIZE];
    char recvbuff[BUFFER_SIZE];
    while (cin >> sendbuff) {
	    if(strcmp(recvbuff,"exit\n")==0)
            break;
        send(client_fd, sendbuff, strlen(sendbuff),0); ///发送
        recv(client_fd, recvbuff, sizeof(recvbuff),0); ///接收
	    cout << recvbuff <<endl;

        memset(sendbuff, 0, BUFFER_SIZE);
        memset(recvbuff, 0, BUFFER_SIZE);
    }

    close(client_fd);
    return 0;
}
