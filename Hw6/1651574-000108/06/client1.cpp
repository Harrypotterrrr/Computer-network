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
#define BUFFER_SIZE (1024 * 4)

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
     if(argc != 5){
        cerr << "Not match parameters!" <<endl;
        exit(EXIT_FAILURE);
    }

    // signal(SIGINT, interuptRes);

    int client_fd;

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    // int port_client = atoi(argv[1]);
    char* ip_server = argv[1];
    int port_server = atoi(argv[2]);
    int readByte = atoi(argv[3]);
    int writeByte = atoi(argv[4]);

    if(readByte > BUFFER_SIZE || writeByte > BUFFER_SIZE){
        cerr << "write or read byte per time is too large!" <<endl;
        exit(EXIT_FAILURE);
    }


    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_server);
    serverAddr.sin_addr.s_addr = inet_addr(ip_server);

    struct sockaddr_in clientAddr;
    // clientAddr.sin_port = htons(port_client);
    // use default, not change
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // clientAddr.sin_addr.s_addr = inet_addr("192.168.60.231");


    if(bind(client_fd,(struct sockaddr *)&clientAddr,sizeof(clientAddr)) == -1)
        myExit();

    if(connect(client_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        myExit();
    else
        cout<<"connection succeed!"<<endl;


    char buffer[BUFFER_SIZE] = "+";
    int read_ctr = 0, write_ctr = 0;
    while(getchar()) {
        read_ctr += read(client_fd, buffer, readByte);
        // if(read_ctr % readByte == 0)
            cout << " Read counter's value have been: " << read_ctr;
        write_ctr += writeByte;
        write(client_fd, buffer, writeByte);
        // if(wrtie_ctr % writeByte == 0)
            cout << " Read counter's value have been: " << write_ctr;
    }

    close(client_fd);
    return 0;
}
