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
#define BUFFER_SIZE (1024)
#define MAX_SIZE (20)
extern int errno;
char * error_messg;


void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg <<endl;
    exit(EXIT_FAILURE);
}

int main(int argc,char* argv[])
{
     if(argc < 4){
        cerr << "Not enough parameters!" <<endl;
        exit(EXIT_FAILURE);
    }

    ///定义sockfd
    int client_fd;

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    int port_client = atoi(argv[1]);
    int port_server = atoi(argv[3]);
    char* ip_server = argv[2];

    ///定义sockaddr_in
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_server);
    serverAddr.sin_addr.s_addr = inet_addr(ip_server);

    struct sockaddr_in clientAddr;
    clientAddr.sin_port = htons(port_client);

    // use default, not change
    // clientAddr.sin_family = AF_INET;
    // clientAddr.sin_addr.s_addr = inet_addr("192.168.60.231");


    if(bind(client_fd,(struct sockaddr *)&clientAddr,sizeof(clientAddr)) == -1)
        myExit();

    if(connect(client_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        myExit();
    else
        cout<<"connection succeed!"<<endl;

    char sendbuff[BUFFER_SIZE] = "\0";
    char recvbuff[BUFFER_SIZE] = "\0";
    
    while (cin.getline(sendbuff,BUFFER_SIZE)) {
	    if(strcmp(recvbuff,"exit\n")==0)
            break;
        send(client_fd, sendbuff, MAX_SIZE,0); ///发送
        recv(client_fd, recvbuff, MAX_SIZE,0); ///接收
	    cout<< "return string: " << recvbuff <<endl;

        memset(sendbuff, 0, BUFFER_SIZE);
        memset(recvbuff, 0, BUFFER_SIZE);
    }

    close(client_fd);
    return 0;
}
