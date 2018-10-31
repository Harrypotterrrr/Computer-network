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

    ///å®šä¹‰sockfd
    int client_fd;

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    int port_client = atoi(argv[1]);
    int port_server = atoi(argv[3]);
    char* ip_server = argv[2];

    ///å®šä¹‰sockaddr_in
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


    char buffer[BUFFER_SIZE] = "+";
    int ctr = 0;
    while(getchar()) {
        ctr += 1000;
        // memset(buffer, '\0', BUFFER_SIZE);
        if(read(client_fd, buffer, 1000) == -1)
            myExit();
        if(ctr % 1000 == 0)
            cout << "Counter's value have been: " << ctr;
    }

    close(client_fd);
    return 0;
}
