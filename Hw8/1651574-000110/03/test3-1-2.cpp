#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#include <signal.h>

#define FIFO_NAME "/tmp/pipe_test"
#define MAX_BUF 100

extern int errno;
char *error_messg;

void myExit(){
    error_messg = strerror(errno);
    std::cerr << error_messg << std::endl;
    exit(EXIT_FAILURE);
}

void savePID()
{
    std::stringstream ss;
    char buffer[MAX_BUF];
    int pipe_fd;
    pid_t pid = getpid();
    
    if(access(FIFO_NAME, F_OK) == -1 && mkfifo(FIFO_NAME, 0777))  // if the pipe doesn't exist
        myExit();

    ss << pid;
    ss >> buffer;

    if((pipe_fd = open(FIFO_NAME, O_WRONLY)) == -1)
        myExit();
    write(pipe_fd, buffer, strlen(buffer) + 1);

    close(pipe_fd);
}

void caputure_con(int sig)
{
    std::cout << sig << " have been successfully caputured " << std::endl;
}

void caputure_end(int sig)
{
    std::cout << sig << " have been successfully caputured " << std::endl;
    std::cout << "Program exit" << std::endl;
    // exit(EXIT_SUCCESS);
}

int main()
{
    savePID();

    for(int i=1 ; i<=64 ; i++)
        signal(i, caputure_con);
    // signal(SIGUSR1, caputure_con);
    // signal(SIGUSR2, caputure_con);
    // signal(SIGALRM, caputure_con);
    // signal(SIGTTIN, caputure_end);

    while(true)
        sleep(1);
    return 0;
}