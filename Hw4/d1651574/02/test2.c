#include <stdio.h>
#include <unistd.h> //  dup(),dup2(),setsid()������ͷ�ļ�
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>  // setsid()������ͷ�ļ�
#include <sys/stat.h>
#include <fcntl.h>

int myPrint()
{
    while(1) {
        printf("1651574\n");
        sleep(5);
    }
    return 0;
}

void myDaemonize()
{
    pid_t pid;
    int i, fd;

    // ����һ���ӽ���
    pid = fork();

    // �����̻�õ��� PID С�� 0, δ�����ɹ�
    if (pid < 0) {
        printf("�ӽ��̴���ʧ�ܣ�\n");
        exit(EXIT_FAILURE);
    }
    // �����̻�ȡ�� PID ���� 0, �����ɹ�, PID ��Ϊ�ӽ��� PID
    else if (pid > 0) {
        printf("�ӽ��̴����ɹ�, PID=%d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // ���²��������ӽ��̽���
    // ʹ�ӽ�������ԭ������, ��Ϊ�µ� session leader �ͽ����� leader
    if ( setsid() < 0 ) {
        printf("�޷�ʹ�ӽ��̳�Ϊ SESSION LEADER!\n");
        exit(EXIT_FAILURE);
    }

    // �ٴ� fork �����, ����ֹ�ӽ���
    pid = fork();
    if (pid < 0) {
        printf("����̴���ʧ�ܣ�\n");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        printf("����̴����ɹ�, PID=%d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // �����ļ�Ȩ���Լ��µĹ���Ŀ¼
    umask(0);
    chdir("/");
    
    // �رմӸ����̼̳������ļ�������
#ifdef RLIMIT_NOFILE
    if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
        printf("getrlimit failed!");
    nfiles = rl.rlim_cur = rl.rlim_max;
    setrlimit(RLIMIT_NOFILE, &rl);
    for(i=3; i<nfiles; i++)
        close(i);
#endif

    // �ض����׼��3���ļ�������
    if (fd = open("/dev/null", O_RDWR) < 0)
        printf("open /dev/null failed!");
    for (i=0; i<3; i++)
        dup2(fd, i);
}

int main()
{
    myDaemonize();
    myPrint();
    return 0;
}