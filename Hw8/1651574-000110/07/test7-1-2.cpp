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
    if(flock(fd, LOCK_EX) != 0)
        std::cerr << "lock the file failed caused by locked by other" << std::endl;

    std::cout << "the file has been locked" << std::endl;

    sleep(3);

    char buffer[MAX_BUF];
    read(fd, buffer, sizeof(buffer));
    int read_len = strlen(buffer) + 1;
    std::cout << read_len << " bytes has been read" <<std::endl;

    flock(fd, LOCK_UN);
    close(fd);
    
    remove(FILE_PATH);
    return 0;
}