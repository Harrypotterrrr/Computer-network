#include <stdio.h>
#include <unistd.h> //  dup(),dup2(),setsid()函数的头文件
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>  // setsid()函数的头文件
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

    // 创建一个子进程
    pid = fork();

    // 父进程获得到的 PID 小于 0, 未创建成功
    if (pid < 0) {
        printf("子进程创建失败！\n");
        exit(EXIT_FAILURE);
    }
    // 父进程获取的 PID 大于 0, 创建成功, PID 即为子进程 PID
    else if (pid > 0) {
        printf("子进程创建成功, PID=%d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // 以下操作即在子进程进行
    // 使子进程脱离原进程组, 成为新的 session leader 和进程组 leader
    if ( setsid() < 0 ) {
        printf("无法使子进程成为 SESSION LEADER!\n");
        exit(EXIT_FAILURE);
    }

    // 再次 fork 孙进程, 并终止子进程
    pid = fork();
    if (pid < 0) {
        printf("孙进程创建失败！\n");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        printf("孙进程创建成功, PID=%d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // 设置文件权限以及新的工作目录
    umask(0);
    chdir("/");
    
    // 关闭从父进程继承来的文件描述符
#ifdef RLIMIT_NOFILE
    if (getrlimit(RLIMIT_NOFILE, &rl) == -1)
        printf("getrlimit failed!");
    nfiles = rl.rlim_cur = rl.rlim_max;
    setrlimit(RLIMIT_NOFILE, &rl);
    for(i=3; i<nfiles; i++)
        close(i);
#endif

    // 重定向标准的3个文件描述符
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