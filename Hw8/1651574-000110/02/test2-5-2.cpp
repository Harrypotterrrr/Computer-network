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

    signal(SIGCHLD, SIG_IGN);   // recycle the test2-5-2

    char buffer[MAX_BUF];
    int data_len;
    int pipe_fd;
    
    if(access(FIFO_NAME, F_OK) == -1 && mkfifo(FIFO_NAME, 0777))  // if the pipe doesn't exist
        myExit();

    if((pipe_fd = open(FIFO_NAME, O_WRONLY)) == -1)
        myExit();

    strcpy(buffer, "0123456789");
    write(pipe_fd, buffer, strlen(buffer) + 1);
    std::cout << strlen(buffer) + 1 << " bytes, test2-5-2 have sent" << std::endl;
    std::cout << buffer << "#, is test2-5-2 sent" <<std::endl;
    close(pipe_fd);


    if((pipe_fd = open(FIFO_NAME, O_RDONLY)) == -1)
        myExit();
    data_len = read(pipe_fd, buffer, sizeof(buffer));
    std::cout << strlen(buffer) + 1 << " bytes, test2-5-2 have recieved" << std::endl;
    std::cout << buffer << "#, is test2-5-2 recieved" <<std::endl;
    close(pipe_fd);

    return 0;
}