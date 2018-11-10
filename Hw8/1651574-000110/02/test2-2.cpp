#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_BUF 100
#define FIFO_NAME "/tmp/pipe_test"

extern int errno;
char *error_messg;

void myExit(){
    error_messg = strerror(errno);
    std::cerr << error_messg << std::endl;
    exit(EXIT_FAILURE);
}

int main(){

    signal(SIGCHLD, SIG_IGN);   // recycle the child process

    char buffer[MAX_BUF];
    int data_len;
    int pipe_fd;
    
    if(access(FIFO_NAME, F_OK) == -1 && mkfifo(FIFO_NAME, 0777))  // if the pipe doesn't exist
        myExit();

    pid_t pid = fork();
    if(pid == -1)
        myExit();
    if(pid == 0){

        if((pipe_fd = open(FIFO_NAME, O_WRONLY)) == -1)
            myExit();

        strcpy(buffer, "9876543210");
        write(pipe_fd, buffer, strlen(buffer) + 1);
        std::cout << strlen(buffer) + 1 << " bytes, child process have sent" << std::endl;
        std::cout << buffer << "#, is child process sent" <<std::endl;
        exit(EXIT_SUCCESS);
    }

    if((pipe_fd = open(FIFO_NAME, O_RDONLY)) == -1)
        myExit();
    data_len = read(pipe_fd, buffer, sizeof(buffer));
    std::cout << strlen(buffer) + 1 << " bytes, parent process have recieved" << std::endl;
    std::cout << buffer << "#, is parent process recieved" <<std::endl;

    close(pipe_fd);
    return 0;
}