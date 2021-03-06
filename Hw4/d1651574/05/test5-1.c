#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <linux/random.h>
#include <errno.h>
#include <sys/wait.h>

#define str_len 1024

void killAll(int sig)
{
    prctl(PR_SET_PDEATHSIG, SIGTERM);
    if(kill(0, SIGTERM) == -1)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}

void childProcess()
{
    char tmp_str[str_len];
    int i = 0;
    for(i = 0 ; i < str_len;i++)
        tmp_str[i] = (char)(rand()%255);

    while(1)
        sleep(1000);
}

int main(int argc, char **argv)
{
    if(argc != 2){
        printf("请输入一个参数表示循环次数\n");
        exit(-1);
    }

    signal(SIGCHLD, SIG_IGN);
    pid_t pid, ppid;
    int k = atoi(argv[1]), i;


    for (i = 0 ; i < k ; i++) {

        pid = fork();
        if (pid < 0) {
            printf("父进程分裂失败!\n");
            break;
            // exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            childProcess();
            exit(EXIT_SUCCESS);
        }
        if(i % 100 == 0)
            printf("已分裂%d个子进程\n", i);
    }

    printf("总计分裂了 %d 个子进程\n", i);

    while(1)
        sleep(1000);

    return 0;
}
