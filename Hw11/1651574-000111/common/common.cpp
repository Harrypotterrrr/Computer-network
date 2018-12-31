#include "common.h"
using namespace std;
const int MYSIGNAL=53;
const int ERR_SIGNAL=63;
const char* SENDER_DAT="sender.dat";
const char* NETWORK_DAT="network.dat";
const char * RECEIVER_DAT="receiver.dat";
int l1_datalen=1024;
int l2_srcport;
int l2_dstport=80;
int l2_offset;
int l2_flag=0x18;
int l3_flag;
int l3_offset=0;
int l3_ttl=64;
int l3_srcip;
int l3_dstip;
int l4_srcmac[6];
int l4_dstmac[6];

int sig_flag=0;
void sig_exec(int num){
    sig_flag=1;
}

void initSig(){
    signal(SIGCHLD, SIG_IGN);
    signal(MYSIGNAL,sig_exec);
}

int randInt(){
    int ans=0;
    for(int i=0; i<4; i++){
        int t=rand()%256;
        ans=(ans<<8)+t;
    }
    return ans;
}

short randShort(){
    short ans=0;
    for(int i=0;i<2;i++){
        short t=rand()%256;
        ans=(ans<<8)+t;
    }
    return ans;
}

char randChar(){
    return rand()%256;
}

void outputFile(const char *p,int len,const char *filename){
    //长度len的p数组十六进制形式输出到文件filename
    FILE *fp=fopen(filename,"w");
    if(fp==NULL){
        cout<<"打开文件"<<filename<<"失败"<<endl;
        exit(0);
    }
    
    for(int i=0;i<len;i++){
        fprintf(fp,"%02x ",int(p[i])&(0x000000ff));
        if(i%16==15)
            fprintf(fp,"\n");
    }
    fclose(fp);
}
int inputFile(char *p,const char* filename){
    FILE *fp=fopen(filename,"r");
    if(fp==NULL){
        cout<<"打开文件"<<filename<<"失败"<<endl;
        exit(0);
    }
    int len=0;
    int temp;
    while(fscanf(fp,"%x",&temp)>0){
        p[len]=temp;
        len++;
    }
    fclose(fp);
    return len;
}
void createDaemon()
{
    int pid;
    if((pid = fork())>0)
        exit(0);
    else if(pid<0)
        exit(1);
    setsid();
    if((pid = fork())>0)
        exit(0);
    else if(pid<0)
        exit(1);
}

short tcpCheck(char *tcp_arr,int source_addr,int dest_addr,short len){
    //tcp计算checksum
    unsigned char *tcp_p = (unsigned char *) tcp_arr;
    unsigned chksum=0;
    for(int i=0;i<len;i+=2)
        chksum+=unsigned(tcp_p[i])<<8;
    for(int i=1;i<len;i+=2)
        chksum+=tcp_p[i];
    chksum+=6;
    unsigned char tmp[10];
    *(unsigned *)tmp=htonl(source_addr);
    *(unsigned *)(tmp+4)=htonl(dest_addr);
    *(unsigned short *)(tmp+8)=htons(len);
    for(int i=0;i<10;i+=2)
        chksum+=unsigned(tmp[i])<<8;
    
    for(int i=1;i<10;i+=2)
        chksum+=tmp[i];
    while(chksum>0xffff)
        chksum=(chksum>>16) + (chksum&0x0000ffff);
    chksum=~chksum;
    //chksum=htons(chksum);
    return short(chksum);
}

short ipCheck(char *ip_arr){
    //ip计算checksum
    int len=20;
    unsigned char *ip_p = (unsigned char *) ip_arr;
    unsigned chksum=0;
    for(int i=0;i<len;i+=2)
        chksum+=unsigned(ip_p[i])<<8;

    for(int i=1;i<len;i+=2)
        chksum+=ip_p[i];

    while(chksum>0xffff)
        chksum=(chksum>>16) + (chksum&0x0000ffff);
    chksum=~chksum;
    //chksum=htons(chksum);
    return short(chksum);
}

int parse_ipv4(char *addr){
    int arr[4];
    unsigned ans=0;
    if(sscanf(addr,"%d.%d.%d.%d",arr,arr+1,arr+2,arr+3)!=4)
        return 0;
    for(int i=0;i<4;i++){
        if(arr[i]<0 || arr[i]>255)
            return 0;
        ans=(ans<<8)+arr[i];
    }
    return ans;

}
int parseMAC(char *addr,int *mac){
    //读入mac
    unsigned ans=0;
    int flag=0;
    if(sscanf(addr,"%x:%x:%x:%x:%x:%x",mac,mac+1,mac+2,mac+3,mac+4,mac+5)!=6)
        flag=1;
    for(int i=0;i<6;i++)
        if(mac[i]<0 || mac[i]>255)
            flag=1;
    if(flag==1){
        for(int i=0;i<6;i++)
            mac[i]=0;
        return 0;
    }
    else
        return 1;
}
void readConfig(){//读取整个配置文件
    char str[1024];
    char stat_str[4][20]={"[应用层]","[传输层]","[网络层]","[数据链路层]"};
    FILE* fp;
    fp=fopen("network.conf","r");
    if(fp==NULL){
        printf("配置文件network.conf打开失败!\n");
        exit(0);
    }
    srand(unsigned(time(0)));
    l2_srcport=rand()%256*256+rand()%256;

    int stat=4;
    while(fscanf(fp,"%s",str)>0){
        if(str[0]=='#'){
            fgets(str,1024,fp);
            continue;
        }
        else if(str[0]=='['){
            stat=4;
            for(int i=0;i<4;i++){
                if(strcmp(stat_str[i],str)==0){
                    stat=i;
                    break;
                }
            }
        }
        else{
            char *p=strchr(str,'=');
            if(p==NULL)
                break;
            if(stat==0){
                if( (p-str==7) && strncmp("datalen",str,7)==0){ 
                    l1_datalen=atoi(p+1);
                    if(l1_datalen<1 || l1_datalen>1460)
                        l1_datalen=1024;
                }
            }else if(stat==1){
                if( (p-str==7) && strncmp("srcport",str,7)==0){ 
                    int temp=atoi(p+1);
                    if(temp>=1 && temp<=65535)
                        l2_srcport=temp;
                }
                else if( (p-str==7) && strncmp("dstport",str,7)==0){  
                    l2_dstport=atoi(p+1);
                    if(l2_dstport<1 || l2_dstport>65535)
                        l2_dstport=80;
                }
                else if( (p-str==6) && strncmp("offset",str,6)==0){  
                    l2_offset=atoi(p+1);
                    if(l2_offset<5 || l2_offset>15)
                        l2_offset=0;
                }
                else if( (p-str==4) && strncmp("flag",str,4)==0){  
                    if(strlen(p+1)!=6)
                        continue;
                    l2_flag=0;
                    for(int i=1;i<=6;i++){
                        l2_flag |= (p[i]-'0')<<(6-i);
                    }
                }

            }else if(stat==2){
                if( (p-str==4) && strncmp("flag",str,4)==0){  
                    if(strlen(p+1)!=3)
                        continue;
                    l3_flag=0;
                    for(int i=1;i<=3;i++){
                        l3_flag |= (p[i]-'0')<<(3-i);
                    }
                }
                else if( (p-str==6) && strncmp("offset",str,6)==0){  
                    l3_offset=atoi(p+1);
                    if(l3_offset<0 || l3_offset>8191)
                        l3_offset=0;
                }
                else if( (p-str==3) && strncmp("ttl",str,3)==0){  
                    l3_ttl=atoi(p+1);
                    if(l3_ttl<0 || l3_ttl>255)
                        l3_ttl=64;
                }
                else if( (p-str==5) && strncmp("srcip",str,5)==0){  
                    l3_srcip=parse_ipv4(p+1);
                }
                else if( (p-str==5) && strncmp("dstip",str,5)==0){  
                    l3_dstip=parse_ipv4(p+1);
                }
            }else if(stat==3){
                if( (p-str==6) && strncmp("srcmac",str,6)==0){  
                    parseMAC(p+1,l4_srcmac);
                }
                else if( (p-str==6) && strncmp("dstmac",str,6)==0){  
                    parseMAC(p+1,l4_dstmac);
                }
            }

        }

    }

}

void printApp(const char *p,int len){
    cout <<"数据信息长度:" << len <<endl;
    for(int i=0;i<len;i++){
        printf("%02x ",int(p[i])&0xff);
        if(i%16==15)
            putchar('\n');
    }
    putchar('\n');
}
void printTcp(const char *p){
    int temp;
    int offset_p;
    printf("TCP包头基本信息：\n");
    temp=ntohs(*(const short *)(p)) & 0x0000ffff;
    printf("sport:%04x(%d)\n",temp,temp);
    temp=ntohs(*(const short *)(p+2)) & 0x0000ffff;
    printf("dport:%04x(%d)\n",temp,temp);
    printf("seq:%08x(%u)\n",ntohl(*(const int *)(p+4)),ntohl(*(const int *)(p+4)));
    printf("ack:%08x(%u)\n",ntohl(*(const int *)(p+8)),ntohl(*(const int *)(p+8)));
    temp=(p[12] & 0x000000ff)>>4;
    offset_p=temp;
    printf("offset:%01x(%d)\n",temp,temp*4);
    printf("reserved:");
    for(int i=3;i>=0;i--)
        printf("%c",(p[12]&(1<<i))?'1':'0');
    for(int i=1;i>=0;i--)
        printf("%c",(p[13]&(1<<(i+6)))?'1':'0');
    printf("(bit)\n");
    printf("flag:");
    for(int i=5;i>=0;i--)
        printf("%c",(p[13]&(1<<i))?'1':'0');
    printf("(bit)\n");
    temp=ntohs(*(const short *)(p+14)) & 0x0000ffff;
    printf("window:%04x(%d)\n",temp,temp);
    temp=ntohs(*(const short *)(p+16)) & 0x0000ffff;
    printf("chksum:%04x\n",temp);
    temp=ntohs(*(const short *)(p+18)) & 0x0000ffff;
    printf("urgptr:%04x\n",temp);
    printf("包头选项信息：\n");
    for(int i=20;i<offset_p*4;i++){
        printf("%02x ",int(p[i])&(0x000000ff));
        if((i-20)%16==15)
            printf("\n");
    }
    printf("\n");
}
void printIp(const char *p){
    int temp;
    printf("IP包头信息：\n");
    temp=(p[0] & 0x000000ff);
    printf("ver+len:%02x\n",temp);
    temp=(p[1] & 0x000000ff);
    printf("tos:%02x\n",temp);
    temp=ntohs(*(const short *)(p+2)) & 0x0000ffff;
    printf("iplen:%04x(%d)\n",temp,temp);
    temp=ntohs(*(const short *)(p+4)) & 0x0000ffff;
    printf("ident:%04x(%d)\n",temp,temp);
    temp=(p[6] & 0x000000ff)>>5;
    char str[4]={0};
    for(int i=0;i<3;i++)
        str[i]=(temp&(1<<(2-i)))?'1':'0';
    printf("flag:%d(%s)\n",temp,str);
    temp=ntohs(*(const short *)(p+6)) & 0x00001fff;
    printf("offset:%04x(%d)\n",temp,temp);
    temp=p[8] & 0x000000ff;
    printf("ttl:%02x(%d)\n",temp,temp);
    temp=p[9] & 0x000000ff;
    printf("proto:%02x\n",temp);
    temp=ntohs(*(const short *)(p+10)) & 0x0000ffff;
    printf("chksum:%04x\n",temp);
    temp=ntohl(*(int *)(p+12));
    printf("srcip:%08x -- %d.%d.%d.%d\n",temp,
        (temp>>24)&0x000000ff,  (temp>>16)&0x000000ff,(temp>>8)&0x000000ff,temp&0x000000ff
        );
    temp=ntohl(*(const int *)(p+16));
    printf("dstip:%08x -- %d.%d.%d.%d\n",temp,
        (temp>>24)&0x000000ff,  (temp>>16)&0x000000ff,(temp>>8)&0x000000ff,temp&0x000000ff
        );
    
}
void printDl(const char *p){
    printf("以太网包头信息：\n");
    printf("dstmac:  %02x:%02x:%02x:%02x:%02x:%02x\n",
        p[0]&0xff,p[1]&0xff,p[2]&0xff,p[3]&0xff,p[4]&0xff,p[5]&0xff);
    printf("srcmac:  %02x:%02x:%02x:%02x:%02x:%02x\n",
        p[6]&0xff,p[7]&0xff,p[8]&0xff,p[9]&0xff,p[10]&0xff,p[11]&0xff);
    int temp=ntohs(*(const short *)(p+12));
    printf("type:%04x\n",temp);
}


//初始化sender准备发送的数据，并写入文件
void initFile(char *p){
    int ne;
    for(int i=0;i<l1_datalen;i++){
        ne=rand()%256;
        p[i]=ne;
    }
    outputFile(p,l1_datalen,SENDER_DAT);
    
}

void sndSolveTcp(char *p){
    *(short *)(p)=htons(l2_srcport); //源端口
    *(short *)(p+2)=htons(l2_dstport);//目标端口
    *(int *)(p+4)=htonl(randInt()); //seq
    *(int *)(p+8)=htonl(randInt());//ack
    *(p+12)=(l2_offset<<4)+0x0;   //offset+高4位reserved
    *(p+13)=(0x0<<6)+l2_flag;    //低2位reserved+flag
    *(short *)(p+14)=htons(randShort()); //window
    *(short *)(p+16)=0; //chksum
    *(short *)(p+18)=0; //urg
    for(int i=20;i<l2_offset*4;i++){
        p[i]=randChar();          //extern
    }
    *(short *)(p+16)=htons(tcpCheck(p,l3_srcip,l3_dstip,l1_datalen+l2_offset*4));  //chksum
    
}

void sndSolveIp(char *p){
    *p=char(0x45); //ver
    *(p+1)=0;  //tos
    *(short *)(p+2)=htons(l1_datalen+4*l2_offset+20); //iplen
    *(short *)(p+4)=htons(randShort()); //ident
    *(short *)(p+6)=htons((l3_flag<<13)+l3_offset); //3位flag + 13位offset
    *(p+8)=l3_ttl;  //ttl
    *(p+9)=6;    //proto
    *(short *)(p+10)=0;  //chksum
    *(int *)(p+12)=htonl(l3_srcip);  //srcip
    *(int *)(p+16)=htonl(l3_dstip);  //dstip

    *(short *)(p+10)=htons(ipCheck(p)); //chksum计算
}

void sndSolveDl(char *p){
    for(int i=0;i<6;i++){  //dstmac
        *(p+i)=l4_dstmac[i];
    }
    for(int i=0;i<6;i++){  //srcmac
        *(p+i+6)=l4_srcmac[i];
    }
    *(short *)(p+12)=htons(0x0800);  //type
}