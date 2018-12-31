#include "../common/common.h"
//char packet[2000];
int shareMemory;
int pidList[5];
#define  MAXSIZE 2048

void shared_receive_app(){
    cout <<"Ӧ�ò�" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    srand(unsigned (time(0)));
    char *p=(char*)shmat(shareMemory,NULL,0);
    //initFile(p);
    int len=*(int*)(p+2000);
    int pre=*(int*)(p+2012);
    outputFile(p+pre,len,RECEIVER_DAT);
    printApp(p+pre,len);
    shmdt(p);
    cout <<"Ӧ�ò����" <<endl;
    //kill(pidList[1],MYSIGNAL);
    
}

void shared_receive_tcp(){
    cout <<"�����" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *p=(char*)shmat(shareMemory,NULL,0);
    int len=*(int*)(p+2000);
    int pre=*(int*)(p+2012);
    char *ptcp=p+pre;
    int tcplen=0;
    if(len>=20){
        tcplen=(ptcp[12] & 0x000000ff)>>4;
    }
    if(len<20 || len-tcplen*4<0){
        printf("TCP��ͷ��Ϣ����\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    *(int*)(p+2000)=len-tcplen*4;
    *(int*)(p+2012)+=tcplen*4;
    //sndSolveTcp(ptcp);
    printTcp(ptcp);
    unsigned short chksum=ntohs(*(short *)(ptcp+16));
    *(short *)(ptcp+16)=0;
    unsigned short chksum_r=tcpCheck(ptcp,*(int*)(p+2004),*(int*)(p+2008),len);
    printf("��֤checksum��%04x(ӦΪ%04x) ",unsigned(chksum),unsigned(chksum_r));
    printf("  %s\n",(chksum==chksum_r)?"һ��":"��һ��");
    if(chksum!=chksum_r){
        printf("TCP��ͷУ��ʹ���\n");
        kill(pidList[0],ERR_SIGNAL);
    }

    shmdt(p);
    cout <<"��������" <<endl;
    kill(pidList[0],MYSIGNAL);
    
}

void shared_receive_ip(){
    cout <<"�����" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *p=(char*)shmat(shareMemory,NULL,0);
    int len=*(int*)(p+2000);
    int pre=*(int*)(p+2012);
    char *pip=p+pre;
    int iplen=0;
    if(len>=20)
        iplen=ntohs(*(const short *)(pip+2)) & 0x0000ffff;
    if(len<20 || len<iplen){
        printf("IP��ͷ��Ϣ����\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    len=iplen;
    //sndSolveIp(pip);
    printIp(pip);
    unsigned short chksum=ntohs(*(short *)(pip+10));
    *(short *)(pip+10)=0;
    unsigned short chksum_r=ipCheck(pip);
    printf("��֤checksum��%04x(ӦΪ%04x) ",unsigned(chksum),unsigned(chksum_r));
    printf("  %s\n",(chksum==chksum_r)?"һ��":"��һ��");
    if(chksum!=chksum_r){
        printf("IP��ͷУ��ʹ���\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    *(int*)(p+2000)=len-20;
    *(int*)(p+2004)=ntohl(*(int *)(pip+12));
    *(int*)(p+2008)=ntohl(*(int *)(pip+16));
    *(int*)(p+2012)+=20;
    shmdt(p);
    cout <<"��������" <<endl;
    kill(pidList[1],MYSIGNAL);
}

void shared_receive_dl(){
    cout <<"������·��" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *p=(char*)shmat(shareMemory,NULL,0);
    int len=*(int*)(p+2000);
    if(len<14){
        printf("��̫����ͷ��Ϣ����\n");
        kill(pidList[0],ERR_SIGNAL);
    }
    printDl(p);
    *(int*)(p+2000)=len-14;
    *(int*)(p+2012)+=14;
    //sndSolveDl(pdl);
    shmdt(p);
    cout <<"������·�����" <<endl;
    kill(pidList[2],MYSIGNAL);

}

void shared_receive_phy(){
    cout <<"�����" <<endl;
    char *p=(char*)shmat(shareMemory,NULL,0);
    int len=inputFile(p,NETWORK_DAT);
    //outputFile(pdl,l1_datalen+20+14+l2_offset*4,NETWORK_DAT);
    printf("����㴫���ֽ�����%d\n",len);
    *(int*)(p+2000)=len;
    *(int*)(p+2012)=0;
    shmdt(p);
    cout <<"��������" <<endl;
    kill(pidList[3],MYSIGNAL);
}

int main(){
    //readConfig();
    createDaemon();
    initSig();
    shareMemory=shmget(0,MAXSIZE,0666|IPC_CREAT);
    //������ͳһ����ռ�
    if(shareMemory<0){
        printf("�����ڴ�ʧ��\n");
        exit(1);
    }
    
    pidList[0]=getpid();
    for(int i=1;i<=4;i++){
        int pid=fork();
        pidList[i]=pid;
        if(pid==0){
            prctl(PR_SET_PDEATHSIG,SIGHUP);
            if(i==1)
                shared_receive_tcp();
            else if(i==2)
                shared_receive_ip();
            else if(i==3)
                shared_receive_dl();
            else if(i==4)
                shared_receive_phy();
            exit(0);
        }
        else if(pidList[i]<0){
            printf("fork���̴���\n");
            //exit(1);
        }
    }
    
    shared_receive_app();
    shmctl(shareMemory,IPC_RMID,0);//������ͳһ�ͷſռ�
    printf("���ս���\n");
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
