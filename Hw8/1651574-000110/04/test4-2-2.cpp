#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define PATH_NAME "."
#define PROJ_ID 0
#define MAX_BUF 100

extern int errno;
char *error_messg;

void myExit(){
    error_messg = strerror(errno);
    std::cerr << error_messg << std::endl;
    exit(EXIT_FAILURE);
}

struct msgBuf
{
    long type;
    char text[MAX_BUF];
};

int main()
{
    int msgID;

    key_t key = ftok(PATH_NAME, PROJ_ID);

    if((msgID = msgget(key, 0644)) == -1)
        myExit();

    msgBuf buffer;

    while(true){

        long msgType = 1;
        if(msgrcv(msgID, (void*)&buffer, sizeof(buffer.text), msgType, 0) == -1)
            myExit();

        std::cout << strlen(buffer.text) + 1 << " bytes, test4-2-2 have recieved" << std::endl;
        std::cout << buffer.text << "#, is test4-1-2 recieved" <<std::endl;

        sleep(1);

        buffer.type = 2;
        strcpy(buffer.text, "9876543210");
        if(msgsnd(msgID, (void*)&buffer, strlen(buffer.text) + 1, 0) == -1)
            myExit();
        
        std::cout << strlen(buffer.text) + 1 << " bytes, test4-2-2 have sent" << std::endl;
        std::cout << buffer.text << "#, is test4-2-2 sent" <<std::endl;
    }


    msgctl(msgID, IPC_RMID, 0); // close the message queue

    return 0;
}