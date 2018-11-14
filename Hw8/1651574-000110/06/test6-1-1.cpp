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
#include <sys/un.h>
#include <signal.h>

using namespace std;


#define MAX_FD   20
#define BUFFER_SIZE (1024)
#define PATH_NAME "/tmp/test"

extern int errno;
char * error_messg;

void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg <<endl;
    exit(EXIT_FAILURE);
}

// void setReusePort(int *fd)
// {
//     int enable = 1;
//     if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
//         myExit();
// }

void removeFile(int sig){
    remove(PATH_NAME);
    cout << "socket has been removed\n";
    exit(EXIT_SUCCESS);
}

int main()
{
    signal(SIGINT, removeFile);
    signal(SIGKILL, removeFile);
    signal(SIGTERM, removeFile);
    signal(SIGQUIT, removeFile);

    int server_fd, connect_fd;
    struct sockaddr_un myAddr, otherAddr;

    if((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        myExit();

    // setReusePort(&server_fd);

    myAddr.sun_family = AF_UNIX;
    strcpy(myAddr.sun_path, PATH_NAME);

    if(bind(server_fd,(struct sockaddr*)&myAddr,sizeof(myAddr)) == -1)
        myExit();

    if(listen(server_fd, MAX_FD) == -1)
        myExit();
    
    socklen_t len_otherAddr = sizeof(sockaddr_un);
    if((connect_fd = accept(server_fd, (struct sockaddr*)&otherAddr, &len_otherAddr)) == -1)
        myExit();

    const int sendLength = 10;
    char sendbuff[sendLength] = "\0";
    char recvbuff[BUFFER_SIZE] = "\0";
    const char tmp_str[] = "123456789";
    strcpy(sendbuff, tmp_str);
    int ctr_recv_byte = 0, ctr_send_byte = 0;

    while(true){
        int tmp_rtn = send(connect_fd, sendbuff, sendLength, 0);
        if(tmp_rtn == -1)
            myExit();
        ctr_send_byte += tmp_rtn;
        cout << sendbuff << " have sent to test6-1-2: " << endl;
        cout << ctr_send_byte << " bytes have sent "<< endl;

        cout <<"-----------------------"<<endl;
        sleep(1);
        int tmp_recv = recv(connect_fd, recvbuff, sizeof(recvbuff), 0);
        if(tmp_recv == -1)
            myExit();
        ctr_recv_byte += tmp_recv;
        cout << recvbuff << " have recieved from test6-1-2: " << endl;
        cout << ctr_recv_byte << " bytes have recieved" << endl;
        
        cout <<"-----------------------"<<endl;
        sleep(1);
    }

    close(connect_fd);
    close(server_fd);
    return 0;
}
