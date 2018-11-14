#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#include <signal.h>

using namespace std;

#define FIFO_NAME "/tmp/pipe_test"
#define MAX_BUF 100

extern int errno;
char *error_messg;

void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg << endl;
    exit(EXIT_FAILURE);
}

int loadPID()
{
    int pipe_fd;
    char buffer[MAX_BUF];
    if((pipe_fd = open(FIFO_NAME, O_RDONLY)) == -1)
        myExit();

    int data_len = read(pipe_fd, buffer, sizeof(buffer));

    close(pipe_fd);

    return atoi(buffer);
}
int main()
{
    int pid = loadPID();

    while(true){
        for(int i=34 ; i<=64 ; i++){
            kill(pid, i);
            cout << "No. " << i << " signal has been sent from " << getpid() << endl;
            // cout <<strsignal(i)<<endl;
            sleep(1);
        }
    }
    return 0;
}