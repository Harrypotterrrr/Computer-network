﻿#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void killAll(int sig)
{
    // if (sig != SIGTERM && sig != SIGINT && sig != SIGUP)
    //     return;

    if(kill(0, SIGTERM) == -1)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}

void childProcess()
{
    pid_t pid, ppid;
    int i;

    signal(SIGCHLD, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGHUP, SIG_DFL);

    while (1) {
        ppid = getppid();
        pid = getpid();
        printf("%d+%d+1651574+sub\n", (int)ppid, (int)pid);
        sleep(15);
    }
}

int main()
{
    pid_t pid, ppid;
    int i;

    signal(SIGCHLD, SIG_IGN);
    signal(SIGTERM, killAll);
    signal(SIGINT, killAll);
    signal(SIGHUP, killAll);


    for (i = 0 ; i < 10 ; i++) {

        // fork()有两个进程返回值
        pid = fork();
        if (pid < 0) {
            printf("创建子进程失败!\n");
            exit(EXIT_FAILURE);
        }
        // 如果当前为子进程
        if (pid == 0) {
            childProcess();
            exit(EXIT_SUCCESS);
        }
        sleep(3);
    }

    while(1) {
        ppid = getppid();
        pid = getpid();
        printf("%d+%d+1651574+main\n", (int)ppid, (int)pid);
        sleep(5);
    }
    return 0;
}