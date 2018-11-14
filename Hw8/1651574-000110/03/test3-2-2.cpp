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
#define FILE_PATH "/tmp/file_test"
#define MAX_BUF 100
#define SPE_SIG 64

extern int errno;
char *error_messg;

int otherPID;
int ctr = 0;
char buffer[MAX_BUF];

void myExit(){
    error_messg = strerror(errno);
    std::cerr << error_messg << std::endl;
    exit(EXIT_FAILURE);
}

int loadPID(int fd)
{
    if((fd = open(FIFO_NAME, O_RDONLY)) == -1)
        myExit();

    int data_len = read(fd, buffer, sizeof(buffer));

    close(fd);

    return(atoi(buffer));
}

void savePID(int fd)
{
    std::stringstream ss;
    pid_t pid = getpid();
    ss << pid;
    ss >> buffer;

    if((fd = open(FIFO_NAME, O_WRONLY)) == -1)
        myExit();
    write(fd, buffer, strlen(buffer) + 1);
    // std::cout << buffer << "# has been successfully written" << std::endl;
    close(fd);
}

int getPID()
{
    char buffer[MAX_BUF];
    int pipe_fd;

    int rtn = loadPID(pipe_fd);

    savePID(pipe_fd);

    return rtn;
}


void writeFile(int fd)
{
    sprintf(buffer, "\"*No. %d* 9876543210\"", ctr++);
    if(write(fd, buffer, strlen(buffer) + 1) == -1)
        myExit();
    std::cout << buffer <<"# is the str sent" << std::endl;
}

void readFile(int fd)
{
    if(read(fd, buffer, sizeof(buffer)) == -1)
        myExit();
    std::cout << buffer << "# is the str recieved" << std::endl;
}

void solveFunc(int sig)
{
    int fd;
    char buffer[MAX_BUF];

    if((fd = open(FILE_PATH, O_RDWR)) == -1)
        myExit();

    std::cout << "--------------------------------" << std::endl;
    std::cout << "[No. " << ctr <<"]" << std::endl;
    readFile(fd);
    close(fd);
    if((fd = open(FILE_PATH, O_RDWR)) == -1)
        myExit();
    writeFile(fd);
    kill(otherPID, SPE_SIG);
    close(fd);
    sleep(1);
}

int main()
{
    signal(SPE_SIG, solveFunc);

    otherPID = getPID();
    std::cout<< "my PID: " << getpid() <<std::endl;
    std::cout<< "the other's PID: " << otherPID << std::endl;

    while(true)
        sleep(1);

    return 0;
}