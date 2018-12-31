#include "../common/common.h"
//char packet[2000];
int shareMemory;
int pidList[5];
#define  MAXSIZE 2048

void shared_send_app(){
    cout <<"Ӧ�ò�" <<endl;
    srand(unsigned (time(0)));
    char *p=(char*)shmat(shareMemory,NULL,0);
    initFile(p+20+14+l2_offset*4);
    printApp(p+20+14+l2_offset*4,l1_datalen);
    shmdt(p);
    cout <<"Ӧ�ò����" <<endl;
    kill(pidList[1],MYSIGNAL);
    
}

void shared_send_tcp(){
    cout <<"�����" <<endl;

    while(sig_flag==0){
        sleep(1);
    }
    char *p=(char*)shmat(shareMemory,NULL,0);
    sndSolveTcp(p+14+20);
    printTcp(p+14+20);
    shmdt(p);
    cout <<"��������" <<endl;
    kill(pidList[2],MYSIGNAL);
    
}

void shared_send_ip(){
    cout <<"�����" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *p=(char*)shmat(shareMemory,NULL,0);
    sndSolveIp(p+14);
    printIp(p+14);
    shmdt(p);
    cout <<"��������" <<endl;
    kill(pidList[3],MYSIGNAL);
}

void shared_send_dl(){
    cout <<"������·��" <<endl;
    printf("��ʼ������·��\n");
    while(sig_flag==0){
        sleep(1);
    }
    char *p=(char*)shmat(shareMemory,NULL,0);
    sndSolveDl(p);
    printDl(p);
    shmdt(p);
    cout <<"������·�����" <<endl;

    kill(pidList[4],MYSIGNAL);

}

void shared_send_phy(){
    cout <<"�����" <<endl;
    while(sig_flag==0){
        sleep(1);
    }
    char *p=(char*)shmat(shareMemory,NULL,0);
    int len=l1_datalen+20+14+l2_offset*4;
    int pad=60-len;
    while(len<60){
        p[len++]='\0';
    }
    outputFile(p,len,NETWORK_DAT);
    printf("����㴫���ֽ�����%d\n",len);
    if(pad>0){
        printf("��������ֽڣ�%d\n",pad);
    }
    shmdt(p);
    cout <<"��������" <<endl;
}

int main(){
    readConfig();
    createDaemon();
    initSig();
    shareMemory=shmget(0,MAXSIZE,0666|IPC_CREAT);
    //������ͳһ����ռ�
    if(shareMemory<0){
        printf("�����ڴ�ʧ��\n");
        exit(1);
    }
    
    pidList[4]=getpid();
    for(int i=3;i>=0;i--){
        int pid=fork();
        pidList[i]=pid;
        if(pid==0){
            prctl(PR_SET_PDEATHSIG,SIGHUP);
            if(i==0)
                shared_send_app();
            else if(i==1)
                shared_send_tcp();
            else if(i==2)
                shared_send_ip();
            else if(i==3)
                shared_send_dl();
            exit(0);
        }
        else if(pidList[i]<0){
            printf("fork���̴���\n");
            //exit(1);
        }
    }
    shared_send_phy();
    shmctl(shareMemory,IPC_RMID,0);//������ͳһ�ͷſռ�
    printf("���ͽ���\n");
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
