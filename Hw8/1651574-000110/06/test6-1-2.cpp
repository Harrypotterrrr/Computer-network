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


#define BUFFER_SIZE (100)
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
//     int enable = 0;
//     if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
//         myExit();
// }

void removeFile(int sig){
    remove(PATH_NAME);
    cout << "socket has been removed\n";
    exit(EXIT_SUCCESS);
}

int main(int argc,char* argv[])
{
    signal(SIGINT, removeFile);
    signal(SIGKILL, removeFile);
    signal(SIGTERM, removeFile);
    signal(SIGQUIT, removeFile);

    int client_fd;
    struct sockaddr_un otherAddr;

    if((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        myExit();

    // setReusePort(&client_fd);

    otherAddr.sun_family = AF_UNIX;
    strcpy(otherAddr.sun_path, PATH_NAME);

    // unlink(otherAddr.sun_path);
    if(connect(client_fd, (struct sockaddr *)&otherAddr, sizeof(otherAddr)) == -1)
        myExit();

    const int sendLength = 10;
    char sendbuff[sendLength] = "\0";
    char recvbuff[BUFFER_SIZE] = "\0";
    const char tmp_str[] = "987654321";
    strcpy(sendbuff, tmp_str);
    int ctr_recv_byte = 0, ctr_send_byte = 0;

    while(true){
        int tmp_recv = recv(client_fd, recvbuff, sizeof(recvbuff), 0);
        // MSG_WAITALL will let the recv plugged until the buffer full
        if(tmp_recv == -1)
            myExit();
        ctr_recv_byte += tmp_recv;
        cout << recvbuff << " have recieved from test6-1-1: " << endl;
        cout << ctr_recv_byte << " bytes have recieved" << endl;

        cout <<"-----------------------"<<endl;
        sleep(1);
        int tmp_rtn = send(client_fd, sendbuff, sendLength, 0);
        if(tmp_recv == -1)
            myExit();
        ctr_send_byte += tmp_rtn;
        cout << sendbuff << " have sent to test6-1-1: " << endl;
        cout << ctr_send_byte << " bytes have sent "<< endl;

        cout <<"-----------------------"<<endl;
        sleep(1);
    }

    close(client_fd);
    return 0;
}
