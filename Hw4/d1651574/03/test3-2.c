#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void childProcess()
{
    pid_t pid, ppid;
    int i;
    for (i=0 ; i < 3 ; i++) {
        ppid = getppid();
        pid = getpid();
        printf("%d+%d+1651574+sub\n", (int)ppid, (int)pid);
        sleep(25);
    }
}

int main()
{
    pid_t pid, ppid;
    int i;

    signal(SIGCHLD, SIG_IGN);

    for (i = 0 ; i < 10 ; i++) {

        // fork()���������̷���ֵ
        pid = fork();
        if (pid < 0) {
            printf("�����ӽ���ʧ��!\n");
            exit(EXIT_FAILURE);
        }
        // �����ǰΪ�ӽ���
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