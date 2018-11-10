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

    if((msgID = msgget(key, IPC_CREAT | 0644)) == -1)
        myExit();   // create the message queue

    // saveMsgID(msgID);

    msgBuf buffer;
    while(true){
        buffer.type = 1;

        strcpy(buffer.text, "0123456789");
        if(msgsnd(msgID, (void*)&buffer, strlen(buffer.text) + 1, 0) == -1)
            myExit();
        
        std::cout << strlen(buffer.text) + 1 << " bytes, test4-1-1 have sent" << std::endl;
        std::cout << buffer.text << "#, is test4-1-1 sent" <<std::endl;

        sleep(1);

        long msgType = 2;
        if(msgrcv(msgID, (void*)&buffer, sizeof(buffer.text), msgType, 0) == -1)
            myExit();

        std::cout << strlen(buffer.text) + 1 << " bytes, test4-2-1 have recieved" << std::endl;
        std::cout << buffer.text << "#, is test4-2-1 recieved" <<std::endl;
    }

    msgctl(msgID, IPC_RMID, 0); // close the message queue

    return 0;
}