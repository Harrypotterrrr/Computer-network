#include "../common/common.h"
//char packet[2000];
int shareMemory[4];
int pidList[5];
#define  MAXSIZE 2048

void copySendApp(){
    cout <<"应用层" <<endl;
    srand(unsigned (time(0)));
    char *p=(char*)shmat(shareMemory[0],NULL,0);
    initFile(p);
    printApp(p,l1_datalen);
    shmdt(p);
    cout <<"应用层结束" <<endl;
    kill(pidList[1],MYSIGNAL);
    
}

void copySendTcp(){
    cout <<"传输层" <<endl;
    while(sig_flag==0)
        sleep(1);
    
    char *pdata=(char*)shmat(shareMemory[0],NULL,0);
    char *ptcp=(char*)shmat(shareMemory[1],NULL,0);
    memcpy(ptcp+l2_offset*4,pdata,l1_datalen);
    sndSolveTcp(ptcp);
    printTcp(ptcp);
    shmdt(pdata);
    shmdt(ptcp);
    cout <<"传输层结束" <<endl;
    kill(pidList[2],MYSIGNAL);
    
}

void copySendIp(){
    cout <<"网络层" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *ptcp=(char*)shmat(shareMemory[1],NULL,0);
    char *pip=(char*)shmat(shareMemory[2],NULL,0);
    memcpy(pip+20,ptcp,l1_datalen+l2_offset*4);
    sndSolveIp(pip);
    printIp(pip);
    shmdt(pip);
    shmdt(ptcp);
    cout <<"网络层结束" <<endl;
    kill(pidList[3],MYSIGNAL);
}

void copySendDl(){
    cout <<"数据链路层" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *pip=(char*)shmat(shareMemory[2],NULL,0);
    char *pdl=(char*)shmat(shareMemory[3],NULL,0);
    memcpy(pdl+14,pip,l1_datalen+l2_offset*4+20);
    sndSolveDl(pdl);
    printDl(pdl);
    shmdt(pip);
    shmdt(pdl);
    cout <<"数据链路层结束" <<endl;
    kill(pidList[4],MYSIGNAL);

}

void copySendPhy(){
    cout <<"物理层" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *pdl=(char*)shmat(shareMemory[3],NULL,0);
    int len=l1_datalen+20+14+l2_offset*4;
    int pad=60-len;
    while(len<60){
        pdl[len++]='\0';
    }
    outputFile(pdl,len,NETWORK_DAT);
    printf("物理层传输字节数：%d\n",len);
    if(pad>0){
        printf("包含填充字节：%d\n",pad);
    }
    shmdt(pdl);
    cout <<"物理层结束" <<endl;
}

int main(){
    readConfig();
    createDaemon();
    initSig();
    for(int i=0; i<4; i++){ //父进程统一申请空间
        shareMemory[i]=shmget(0,MAXSIZE,0666|IPC_CREAT);
        if(shareMemory[i]<0){
            i--;
            while(i--)
                shmctl(shareMemory[i],IPC_RMID,0);
            cout <<"内存申请失败!" <<endl;
            exit(1);
        }
    }
    
    pidList[4]=getpid();
    for(int i=3; i>=0; i--){
        int pid=fork();
        pidList[i]=pid;
        if(pid==0){
            prctl(PR_SET_PDEATHSIG,SIGHUP);
            if(i==0)
                copySendApp();
            else if(i==1)
                copySendTcp();
            else if(i==2)
                copySendIp();
            else if(i==3)
                copySendDl();
            exit(0);
        }
        else if(pidList[i]<0){
            cout << "fork error!" <<endl;
            //exit(1);
        }
    }
    copySendPhy();
    //父进程统一释放空间
    for(int i=0; i<4; i++) 
        shmctl(shareMemory[i],IPC_RMID,0);
    cout << "完成发送" <<endl;
    return 0;
}
