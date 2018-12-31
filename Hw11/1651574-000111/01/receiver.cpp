#include "../common/common.h"

//char packet[2000];
int shareMemory[4];
int pidList[5];
#define  MAXSIZE 2048

void copyRecvApp(){
    cout <<"应用层" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    srand(unsigned (time(0)));
    char *p=(char*)shmat(shareMemory[0],NULL,0);
    //initFile(p);
    int len=*(int*)(p+2000);
    outputFile(p,len,RECEIVER_DAT);
    printApp(p,len);
    shmdt(p);
    cout <<"应用层结束" <<endl;
    //kill(pidList[1],MYSIGNAL);
    
}

void copyRecvTcp(){
    cout <<"传输层" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *pdata=(char*)shmat(shareMemory[0],NULL,0);
    char *ptcp=(char*)shmat(shareMemory[1],NULL,0);
    int len=*(int*)(ptcp+2000);
    
    int tcplen=0;
    if(len>=20){
        tcplen=(ptcp[12] & 0x000000ff)>>4;
    }
    if(len<20 || len-tcplen*4<0){
        printf("TCP包头信息错误\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    memcpy(pdata,ptcp+tcplen*4,len-tcplen*4);
    *(int*)(pdata+2000)=len-tcplen*4;
    //sndSolveTcp(ptcp);
    printTcp(ptcp);
    unsigned short chksum=ntohs(*(short *)(ptcp+16));
    *(short *)(ptcp+16)=0;
    unsigned short chksum_r=tcpCheck(ptcp,*(int*)(ptcp+2004),*(int*)(ptcp+2008),*(int*)(ptcp+2000));
    printf("验证checksum：%04x(应为%04x) ",unsigned(chksum),unsigned(chksum_r));
    printf("  %s\n",(chksum==chksum_r)?"一致":"不一致");
    if(chksum!=chksum_r){
        printf("TCP包头校验和错误\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    shmdt(pdata);
    shmdt(ptcp);
    cout <<"传输层结束" <<endl;
    kill(pidList[0],MYSIGNAL);
    
}

void copyRecvIp(){
    cout <<"网络层" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *ptcp=(char*)shmat(shareMemory[1],NULL,0);
    char *pip=(char*)shmat(shareMemory[2],NULL,0);
    int len=*(int*)(pip+2000);
    
    int iplen=0;
    if(len>=20)
        iplen=ntohs(*(const short *)(pip+2)) & 0x0000ffff;
    if(len<20 || len<iplen){
        printf("IP包头信息错误\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    len=iplen;
    memcpy(ptcp,pip+20,len-20);
    //sndSolveIp(pip);
    printIp(pip);
    unsigned short chksum=ntohs(*(short *)(pip+10));
    *(short *)(pip+10)=0;
    unsigned short chksum_r=ipCheck(pip);
    printf("验证checksum：%04x(应为%04x) ",unsigned(chksum),unsigned(chksum_r));
    printf("  %s\n",(chksum==chksum_r)?"一致":"不一致");
    if(chksum!=chksum_r){
        printf("IP包头校验和错误\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    *(int*)(ptcp+2000)=len-20;
    *(int*)(ptcp+2004)=ntohl(*(int *)(pip+12));
    *(int*)(ptcp+2008)=ntohl(*(int *)(pip+16));
    shmdt(pip);
    shmdt(ptcp);
    cout <<"网络层结束" <<endl;
    kill(pidList[1],MYSIGNAL);
}

void copyRecvDl(){
    cout <<"数据链路层" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *pip=(char*)shmat(shareMemory[2],NULL,0);
    char *pdl=(char*)shmat(shareMemory[3],NULL,0);
    int len=*(int*)(pdl+2000);
    if(len<14){
        printf("以太网包头信息错误\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    printDl(pdl);
    memcpy(pip,pdl+14,len-14);//pdl+14,pip,l1_datalen+l2_offset*4+20);
    *(int*)(pip+2000)=len-14;
    //sndSolveDl(pdl);
    shmdt(pip);
    shmdt(pdl);
    cout <<"数据链路层结束" <<endl;
    kill(pidList[2],MYSIGNAL);

}

void copyRecvPhy(){
    cout <<"物理层" <<endl;
    char *pdl=(char*)shmat(shareMemory[3],NULL,0);
    int len=inputFile(pdl,NETWORK_DAT);
    //outputFile(pdl,l1_datalen+20+14+l2_offset*4,NETWORK_DAT);
    printf("物理层传输字节数：%d\n",len);
    *(int*)(pdl+2000)=len;
    shmdt(pdl);
    cout <<"物理层结束" <<endl;
    kill(pidList[3],MYSIGNAL);
}

int main(){
    //readConfig();
    createDaemon();
    initSig();
    for(int i=0;i<4;i++){ //父进程统一申请空间
        shareMemory[i]=shmget(0,MAXSIZE,0666|IPC_CREAT);
        if(shareMemory[i]<0){
            i--;
            while(i--)
                shmctl(shareMemory[i],IPC_RMID,0);
            cout <<"内存申请失败!" <<endl;
            exit(1);
        }
    }
    
    pidList[0]=getpid();
    for(int i=1;i<=4;i++){
        int pid=fork();
        pidList[i]=pid;
        if(pid==0){
            prctl(PR_SET_PDEATHSIG,SIGHUP);
            if(i==1)
                copyRecvTcp();
            else if(i==2)
                copyRecvIp();
            else if(i==3)
                copyRecvDl();
            else if(i==4)
                copyRecvPhy();
            exit(0);
        }
        else if(pidList[i]<0){
            cout << "fork error!" <<endl;
            //exit(1);
        }
    }
    
    copyRecvApp();
    for(int i=0;i<4;i++) //父进程统一释放空间
        shmctl(shareMemory[i],IPC_RMID,0);
    printf("接收结束\n");
    return 0;

    
    //initFile(packet+20+14+l2_offset*4);
    //sndSolveTcp(packet+14+20);
    //sndSolveIp(packet+14);
    //sndSolveDl(packet);
    //if(l1_datalen+20+14+l2_offset*4>=60)
    //    outputFile(packet,l1_datalen+20+14+l2_offset*4,NETWORK_DAT);
    //else
    //    outputFile(packet,60,"network.dat");
    return 0;
}
