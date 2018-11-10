#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUF 100

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
    int pipe_fd[2][2];
    if(pipe(pipe_fd[0]) == -1)
        myExit();
    if(pipe(pipe_fd[1]) == -1)
        myExit();

    pid_t pid = fork();
    if(pid == -1)
        myExit();
    if(pid == 0){
        strcpy(buffer, "0123456789");
        close(pipe_fd[0][0]); // close the read end
        write(pipe_fd[0][1], buffer, strlen(buffer) + 1);
        std::cout << strlen(buffer) + 1 << " bytes, child process have sent" << std::endl;
        std::cout << buffer << " , is child process sent" <<std::endl;
        
        sleep(1);

        close(pipe_fd[1][1]);
        data_len = read(pipe_fd[1][0], buffer, MAX_BUF);
        std::cout << data_len << " bytes, child process have recieved" << std::endl;
        std::cout << buffer << " , is child process recieved" <<std::endl;
        exit(EXIT_SUCCESS);
    }

    
    close(pipe_fd[0][1]); // close the write end
    data_len = read(pipe_fd[0][0], buffer, MAX_BUF);
    std::cout << data_len << " bytes, parent process have recieved" << std::endl;
    std::cout << buffer << " , is parent process recieved" <<std::endl;


    sleep(1);

    strcpy(buffer, "9876543210");
    close(pipe_fd[1][0]); // close the read end
    write(pipe_fd[1][1], buffer, strlen(buffer) + 1);
    std::cout << strlen(buffer) + 1 << " bytes, parent process have sent" << std::endl;
    std::cout << buffer << " , is parent process sent" <<std::endl;

   
    return 0;
}