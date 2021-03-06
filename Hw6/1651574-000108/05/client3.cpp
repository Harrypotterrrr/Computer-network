#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
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

int main(int argc,char* argv[])
{
     if(argc < 4){
        cerr << "Not enough parameters!" <<endl;
        exit(EXIT_FAILURE);
    }

    // signal(SIGINT, interuptRes);

    int client_fd;

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    int nSendBuf = 32 * 1024; //����Ϊ32K
    setsockopt(client_fd,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));

    int port_client = atoi(argv[1]);
    int port_server = atoi(argv[3]);
    char* ip_server = argv[2];

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


    char sendbuff[BUFFER_SIZE] = "+";
    int ctr = 0;
    while (true) {
        ctr++;
        if(write(client_fd, sendbuff, 1) <= 0){
            cout << "The Total counter reaches: " << ctr << endl;
            break;
        }
        if(ctr % 1000 == 0)
            cout << "Counter's value have been: " << ctr << endl;
        // memset(sendbuff, '\0', BUFFER_SIZE);
    }

    close(client_fd);
    return 0;
}
