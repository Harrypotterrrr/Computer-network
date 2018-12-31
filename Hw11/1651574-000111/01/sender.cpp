#include "../common/common.h"
//char packet[2000];
int shareMemory[4];
int pidList[5];
#define  MAXSIZE 2048

void copySendApp(){
    cout <<"Ӧ�ò�" <<endl;
    srand(unsigned (time(0)));
    char *p=(char*)shmat(shareMemory[0],NULL,0);
    initFile(p);
    printApp(p,l1_datalen);
    shmdt(p);
    cout <<"Ӧ�ò����" <<endl;
    kill(pidList[1],MYSIGNAL);
    
}

void copySendTcp(){
    cout <<"�����" <<endl;
    while(sig_flag==0)
        sleep(1);
    
    char *pdata=(char*)shmat(shareMemory[0],NULL,0);
    char *ptcp=(char*)shmat(shareMemory[1],NULL,0);
    memcpy(ptcp+l2_offset*4,pdata,l1_datalen);
    sndSolveTcp(ptcp);
    printTcp(ptcp);
    shmdt(pdata);
    shmdt(ptcp);
    cout <<"��������" <<endl;
    kill(pidList[2],MYSIGNAL);
    
}

void copySendIp(){
    cout <<"�����" <<endl;
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
    cout <<"��������" <<endl;
    kill(pidList[3],MYSIGNAL);
}

void copySendDl(){
    cout <<"������·��" <<endl;
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
    cout <<"������·�����" <<endl;
    kill(pidList[4],MYSIGNAL);

}

void copySendPhy(){
    cout <<"�����" <<endl;
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
    printf("����㴫���ֽ�����%d\n",len);
    if(pad>0){
        printf("��������ֽڣ�%d\n",pad);
    }
    shmdt(pdl);
    cout <<"��������" <<endl;
}

int main(){
    readConfig();
    createDaemon();
    initSig();
    for(int i=0; i<4; i++){ //������ͳһ����ռ�
        shareMemory[i]=shmget(0,MAXSIZE,0666|IPC_CREAT);
        if(shareMemory[i]<0){
            i--;
            while(i--)
                shmctl(shareMemory[i],IPC_RMID,0);
            cout <<"�ڴ�����ʧ��!" <<endl;
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
    //������ͳһ�ͷſռ�
    for(int i=0; i<4; i++) 
        shmctl(shareMemory[i],IPC_RMID,0);
    cout << "��ɷ���" <<endl;
    return 0;
}
