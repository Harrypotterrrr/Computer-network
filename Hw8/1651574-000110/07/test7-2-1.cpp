#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/file.h>

#define FILE_PATH "/tmp/test.txt"
#define MAX_BUF 100

extern int errno;
char * error_messg;

void myExit(){
    error_messg = strerror(errno);
    std::cerr << error_messg << std::endl;
    exit(EXIT_FAILURE);
}

void setLock(int fd, int type)
{
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // lock the whole file
    lock.l_type = type;

    fcntl(fd, F_GETLK, &lock);
    if (lock.l_type != F_UNLCK) {  
        if (lock.l_type == F_RDLCK) 
            std::cout << "Read lock already set by " << lock.l_pid << std::endl;
        else if (lock.l_type == F_WRLCK)   
            std::cout << "write lock already set by " << lock.l_pid << std::endl;
        do{
            std::cout << "wait 1sec for the file unlocked" << std::endl;
            sleep(1);
            fcntl(fd, F_GETLK, &lock);
        }while(lock.l_type != F_UNLCK);
    }

    lock.l_type = type;

    if(fcntl(fd, F_SETLK, &lock) == -1)
        myExit();
    
    switch(lock.l_type){
        case F_RDLCK:  
            std::cout << "read lock is set by " << getpid() << std::endl; break;
        case F_WRLCK:  
            std::cout << "write lock is set by " << getpid() << std::endl; break;
        case F_UNLCK:
            std::cout << "unlock set by " << getpid() << std::endl; break;
        default: break;
    }
}

int main()
{
    int fd;
    if((fd = open(FILE_PATH, O_CREAT| O_WRONLY, 0666)) == -1)
        myExit();
    
    int flag = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK); // set non block

    std::cout << "wait for locking..." << std::endl;
    setLock(fd, F_WRLCK);

    std::cout << "sleeping..." << std::endl;
    sleep(3);

    char buffer[MAX_BUF];
    strcpy(buffer, "0123456789");
    int write_len = write(fd, buffer, strlen(buffer) + 1);
    std::cout << write_len << " bytes has been written" <<std::endl;

    setLock(fd, F_UNLCK);
    close(fd);

    return 0;
}