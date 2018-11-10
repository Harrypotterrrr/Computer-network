#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define PATH_NAME "."
#define PROJ_ID 0
#define MAX_BUF 100

#define KEY 100

extern int errno;
char *error_messg;

void myExit(){
    error_messg = strerror(errno);
    std::cerr << error_messg << std::endl;
    exit(EXIT_FAILURE);
}

struct shmBuf
{
    char text[MAX_BUF];
};
shmBuf *buffer;

int main()
{
    int shmID;

    key_t key = ftok(PATH_NAME, PROJ_ID);
    if((shmID = shmget(key, sizeof(shmBuf), 0644)) == -1)
        myExit();   // create the share memory

    if((buffer = (shmBuf*)shmat(shmID, NULL, 0)) == NULL)
        myExit();

    strcpy(buffer->text, "9876543210");
    while(true){
        
        std::cout << strlen(buffer->text) + 1 << " bytes, test5-2-2 have loaded" << std::endl;
        std::cout << buffer->text << "#, is test5-2-2 loaded" <<std::endl;

        std::cout << "--------------------------" << std::endl;
        sleep(1);

        buffer->text[9] = (buffer->text[9] - '0' + 1) % 10 + '0';
        
        std::cout << strlen(buffer->text) + 1 << " bytes, test5-2-2 have stored" << std::endl;
        std::cout << buffer->text << "#, is test5-2-2 stored" << std::endl;

        std::cout << "--------------------------" << std::endl;
        sleep(1);
    }

    shmdt(buffer);  // depart the memory from the present process
    shmctl(shmID, IPC_RMID, 0); // delete the share memory

    return 0;
}