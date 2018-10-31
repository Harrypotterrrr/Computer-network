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

void get_all_local_IP() 
{
    int self_fd;
    char *ip, *buff = new char[BUFFER_SIZE];
    struct ifconf ifc;
    
    ifc.ifc_len = BUFFER_SIZE ;     // the max length of the ifc_buffer
    ifc.ifc_buf = (caddr_t) buff;

    if ((self_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        myExit();

    ioctl(self_fd, SIOCGIFCONF, &ifc);
    close(self_fd);

    struct ifreq *ifr = (struct ifreq*) buff;
    int i = ifc.ifc_len / sizeof(struct ifreq); // afther ioctl, the length of the ifc_buffer will change

    cout << "IP includes:" << endl;
    for( ; i>0 ; i--, ifr++){
        ip = inet_ntoa(((struct sockaddr_in*)&(ifr->ifr_addr))->sin_addr);
        cout << ip << endl;
    }

    delete buff;
    return;
}

int main(int argc,char* argv[])
{
     if(argc != 4){
        cerr << "Not match parameters!" <<endl;
        exit(EXIT_FAILURE);
    }

    int server_fd;
    get_all_local_IP();

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    // if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    //     myExit();

    int port = atoi(argv[1]);
    int readByte = atoi(argv[2]);
    int writeByte = atoi(argv[3]);
    if(readByte > BUFFER_SIZE || writeByte > BUFFER_SIZE){
        cerr << "write or read byte per time is too large!" <<endl;
        exit(EXIT_FAILURE);
    }

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

    int connect_fd;
    struct sockaddr_in client_addr;
    socklen_t len_client_addr = sizeof(client_addr);
    
    if((connect_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len_client_addr)) == -1)
        myExit();

    cout<<"connection succeed!"<<endl;

    // if(getpeername(connect_fd, (struct sockaddr*)&client_addr, &len_client_addr) == -1)
    //     myExit();
    
    // cout<<"client IP:"<<inet_ntoa(c.sin_addr)<<"  port:"<<ntohs(c.sin_port)<<endl;

    cout<<"client IP: "<<inet_ntoa(client_addr.sin_addr)<<"\tport: "<<ntohs(client_addr.sin_port)<<endl;


    char buffer[BUFFER_SIZE] = "+";
    int read_ctr = 0, write_ctr = 0;
    while(getchar()) {
        read_ctr += read(connect_fd, buffer, readByte);
        // if(read_ctr % 10000 == 0)
            cout << " Read counter's value have been: " << read_ctr;
        write_ctr += write(connect_fd, buffer, writeByte);
        // if(write_ctr % writeByte == 0)
            cout << " Read counter's value have been: " << write_ctr;
    }


    close(connect_fd);
    close(server_fd);
    return 0;
}
