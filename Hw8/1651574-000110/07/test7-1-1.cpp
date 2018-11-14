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

#define FILE_PATH "./test.txt"
#define MAX_BUF 100

extern int errno;
char * error_messg;

void myExit(){
    error_messg = strerror(errno);
    std::cerr << error_messg << std::endl;
    exit(EXIT_FAILURE);
}

int main()
{
    int fd;
    if((fd = open(FILE_PATH, O_CREAT | O_RDWR, 0666)) == -1)
        myExit();
    
    std::cout << "wait for locking" << std::endl;
    if(flock(fd, LOCK_EX) != 0){
        std::cerr << "lock the file failed caused by locked by other" << std::endl;
        myExit();
    }

    std::cout << "the file has been locked" << std::endl;

    sleep(3);

    char buffer[MAX_BUF];
    strcpy(buffer, "0123456789");
    int write_len = write(fd, buffer, strlen(buffer) + 1);
    std::cout << write_len << " bytes has been written" <<std::endl;

    flock(fd, LOCK_UN);
    close(fd);

    return 0;
}