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

int ctr_rqt,ctr_rcy, ctr_p;
int max_pid = -1;
int father_pid;

void printResult(int sig)
{
    if(getpid() == father_pid){
        printf("~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("�ܼƷ����� %d ���ӽ���\n", ctr_p);
        printf("���pidΪ%d\n",max_pid);
        printf("������Ϊ: %d, ������Ϊ: %d\n", ctr_rqt, ctr_rcy);
        printf("~~~~~~~~~~~~~~~~~~~~~~\n");
        exit(EXIT_SUCCESS);
    }
}

void childProcess()
{
    signal(SIGCHLD, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGHUP, SIG_DFL);
    char tmp_str[str_len];
    int i = 0;
    for(i = 0 ; i < str_len;i++)
        tmp_str[i] = (char)(rand()%255);

    sleep(1);
}

void recycleCount(int sig)
{
    int status;
    while(1){
        if(waitpid(-1,&status, WNOHANG) > 0)
            ctr_rcy++;
        else
            break;
    }
}

int main(int argc, char **argv)
{
    if(argc != 2){
        printf("������һ��������ʾѭ������\n");
        exit(-1);
    }

    signal(SIGCHLD, &recycleCount);
    signal(SIGINT, &printResult);

    father_pid = getpid();
    pid_t pid, ppid;

    int k = atoi(argv[1]);
    int status;


    for (ctr_p = 0 ; ctr_p < k ; ctr_p++) {
        pid = fork();
        if (pid < 0) {
            printf("�����̷���ʧ�ܣ����Եȴ����ٴ��������!\n");
            sleep(1);
            ctr_p --;
            continue;
        }
        if (pid == 0) {
            childProcess();
            exit(EXIT_SUCCESS);
        }

        ctr_rqt ++;
        max_pid = max_pid > pid ? max_pid : pid;
        
        if(ctr_p % 100 == 0)
            printf("�ѷ���%d���ӽ��̣���ǰ���pidΪ%d����ǰ����%d�����̣�����%d������\n", ctr_p, max_pid, ctr_rqt, ctr_rcy);
    }

    // wait the sub process to end
    while(1)
        if(ctr_rcy==ctr_rqt)
 			break;

    printResult(0);
    return 0;
}
