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

int main(int argc,char* argv[])
{
    ///����sockfd
    int server_fd;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        cerr << "create socket error!"<<endl;
        exit(EXIT_FAILURE);
    }

    ///����sockaddr_in
    int port = atoi(argv[1]);

    struct sockaddr_in serveAddr;
    serveAddr.sin_family = AF_INET;
    //IP��ַ���ó�INADDR_ANY,��ϵͳ�Զ���ȡ������IP��ַ��  
    serveAddr.sin_port = htons(port);
    //���õĶ˿�ΪINADDR_ANY
    serveAddr.sin_addr.s_addr = htonl(INADDR_ANY);    //  host to unsigned long, INADDR_ANY = 0.0.0.0

    if(bind(server_fd,(struct sockaddr *)&serveAddr,sizeof(serveAddr)) == -1) {
        cerr << "bind socket error!"<<endl;
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, QUEUE) == -1) {
        cerr << "listen socket error!"<<endl;
        exit(EXIT_FAILURE);
    }

    ///�ͻ����׽���
    int connect_fd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t len_client_addr = sizeof(client_addr);

    
    ///�ɹ����طǸ������֣�������-1
    if((connect_fd = accept(server_fd, (struct sockaddr*)&client_addr, 0)) < 0) {
        cerr << "accept socket error!"<<endl;
        exit(EXIT_FAILURE);
    }
    else
	    cout<<"connection succeed!"<<endl;
        
    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        if(recv(connect_fd, buffer, BUFFER_SIZE, &len_client_addr) == 0)
            break;  // MSG_WAITALL MSG_DONTWAITALL 0 ???
    }

    close(connect_fd);
    close(server_fd);
    return 0;
}
