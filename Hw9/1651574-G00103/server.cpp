#include "myutil.h"
#include "server.h"

char * error_messg;


ClientInfo clientInfo[MAX_CONNECT]


int server_fd, connect_fd[MAX_CONNECT];
struct sockaddr_in server_addr, client_addr;
fd_set read_fds, write_fds;

int ctr_client = 0;     // the counter of client
bool flag_client[MAX_CONNECT];   // record the valid of client_fd, whether alive or not

char send_buff[MAX_CONNECT][LEN_SEND];   // the buffer of send
char recv_buff[MAX_CONNECT][BUFFER_SIZE];    // the buffer of recv

struct timeval wait_time;   // select wait_time

int ctr_recv_byte[MAX_CONNECT], ctr_send_byte[MAX_CONNECT];   // counter of recv/send byte

int max_fd; // record the maximum of fd


void myExit(){
    error_messg = strerror(errno);
    cerr << error_messg <<endl;
    exit(EXIT_FAILURE);
}

void writeWait(int sig)
{
    FD_ZERO(&write_fds);
    for(int i=0 ; i<ctr_client ; i++){
        if(!flag_client[i])
            continue;
        FD_SET(connect_fd[i], &write_fds);
    }
    alarm(1);
}

void init()
{
    max_fd = server_fd;
    for(int i=0 ; i<MAX_CONNECT ; i++){
        ctr_client = 0;
        flag_client[i] = false;
        strcpy(send_buff[i],"123456789");
        recv_buff[i][0] = '\0';
        ctr_recv_byte[i] = ctr_send_byte[i] = 0;
    }
    if(!flag_block){
        wait_time.sec = 1;
        wait_time.usec = 0;
    }
}

void printServerInfo()
{
    cout << "=============================\n";
    cout << "The socket binds to:"<< endl;
    cout << "IP: " << inet_ntoa(server_addr.sin_addr) << endl;
    cout << "port: " << ntohs(server_addr.sin_port) <<endl;
    cout << "==============================\n";
}

void printClientInfo(int no)
{
    cout << "------------------------------"<<endl;
    cout << "[#No. " << no <<"]: connection succeed!"<<endl;
    cout << "[#No. "<< no<<"]: client IP: "<<inet_ntoa(client_addr.sin_addr)<<"\tport: "<<ntohs(client_addr.sin_port)<<endl;
    cout << "------------------------------"<<endl;
}

void setReusePort()
{
    int enable = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
        myExit();
}

void setNonBlock(int *fd, int tag)
{
    int flag;
    if((flag = fcntl(*fd, F_GETFL, 0)) == -1)
        myExit();
    if(fcntl(*fd, F_SETFL, flag | O_NONBLOCK) == -1)
        myExit();

    cout << "set " << (tag == 0 ? "server" : "client") << " non-block succeed" << endl;
}


void sendMsg()
{
    for(int i=0 ; i < ctr_client ; i++){
        if(!flag_client[i])
            continue;
        if(FD_ISSET(connect_fd[i], &write_fds)){
            FD_CLR(connect_fd[i], &write_fds);
            int tmp_rtn = send(connect_fd[i], send_buff[i], LEN_SEND, MSG_DONTWAIT);
            if(tmp_rtn < 1){
                cout<<"[No. " << i <<"]: the end of conversation" <<endl;
                flag_client[i] = false;
                close(connect_fd[i]);
            }

            ctr_send_byte[i] += tmp_rtn;
            cout<<"[No. " << i <<"]: send to client: " << send_buff[i] << endl;
            cout<<"[No. " << i <<"]: "<< ctr_send_byte[i] << " bytes have sent " << endl;
        }
    }
}

void recvMsg()
{
    for(int i=0 ; i < ctr_client ; i++){
        if(!flag_client[i])
            continue;
        if(FD_ISSET(connect_fd[i], &read_fds)){
            FD_CLR(connect_fd[i], &read_fds);
            int tmp_rtn = recv(connect_fd[i], recv_buff[i], sizeof(recv_buff[i]), MSG_DONTWAIT);
            if(tmp_rtn < 1){
                cout << "------------------------------"<<endl;
                cout<<"[No. " << i <<"]: the end of conversation" <<endl;
                cout << "------------------------------"<<endl;
                flag_client[i] = false;
                close(connect_fd[i]);
            }
            
            ctr_recv_byte[i] += tmp_rtn;
            cout<<"[No. " << i <<"]: recieve from client: " << recv_buff[i] << endl;
            cout<<"[No. " << i <<"]: "<< ctr_recv_byte[i] << " bytes have recieved " << endl;
        }
    }
}

void createNewConnect()
{
    socklen_t len_client_addr = sizeof(client_addr);
    int i = ctr_client++;
    if((connect_fd[i] = accept(server_fd, (struct sockaddr*)&client_addr, &len_client_addr)) == -1)
        myExit();
    
    max_fd = max_fd > connect_fd[i] ? max_fd : connect_fd[i];

    printClientInfo(i);

    setNonBlock(&connect_fd[i], 1);

    flag_client[i] = true;


    clientInfo[i].cfd = connect_fd[i];
    clientInfo[i].count = 0;
    memset(clientInfo.msg, 0 ,sizeof(ClinetMSg));
    clientInfo[i].flag = true;
}
bool sSend(int & myCount , int cfd)
{
    int count = myCount /2;
    int MsgLen = strlen(orderMsg[count]);

    bool sndFlag ;
    if(count==GETSTR||count==GETIME)
        MsgLen +=1 ;

    if(count==GETSTR){
        char strMsg [10]= "str";
        int randNum = 32768 +  ((rand()+1e-6)/RAND_MAX*67231);
        sprintf(strMsg+strlen(strMsg),"%d",randNum);
        sndFlag = mySndMsg(cfd, strMsg,strlen(strMsg)+1);
    }
    else
        sndFlag = mySndMsg(cfd,orderMsg[count],MsgLen);
    
    if(sndFlag)
        myCount++;
    return sndFlag;
}

bool sRecv(int & myCount , int cfd, struct ClientMsg * cMsg )
{
    int count = myCount / 2;
    int readNum  ;
    switch(count){
    case GETSTUNO :
        readNum = recv(cfd,&(cMsg->StuNo),sizeof(cMsg->StuNo),0);
        break;
    case GETPID :
        readNum = recv(cfd,&(cMsg->clientPid),sizeof(cMsg->clientPid),0);
        break;
    case GETIME :
        readNum  = recv(cfd, (cMsg->clientTime),sizeof(cMsg->clientTime),0);
        if(readNum>0)
            cMsg->clientTime[readNum]=0;
        break;
    case GETSTR :
        readNum = recv(cfd,(cMsg->randomMsg),cMsg->randomMsgLen,0);
        if(readNum==cMsg->randomMsgLen)
            cMsg->randomMsg[cMsg->randomMsgLen]=0;
        else
            readNum = -1;
        break;
    }
    if(readNum<=0)
        return false;

    myCount ++ ;
    return true;
}

int main(int argc,char* argv[])
{
    parseCMD(argc, argv , false);


    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        myExit();

    setReusePort();

    if(!flag_block)
        setNonBlock(&server_fd, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == -1)
        myExit();

    printServerInfo();

    if(listen(server_fd, max_num) == -1)
        myExit();
    
    cout << "server is listening" <<endl;
    
    init();

    if(flag_fork){
        pid_t pid = fork();
        if (pid < 0)
            myExit();
        else if(pid > 0){
            cout << "recieve new connection require!" <<endl;

        }
        else{
            createNewConnect();

            void *p = NULL;
            if(flag_block)
                p = &wait_time;

            while(true){
                FD_ZERO(&read_fds);
                FD_SET(server_fd, &read_fds);
                select(max_fd + 1, &read_fds, &write_fds, p){
                    //send() write()
                }
            }
        }
    }
    else{
        signal(SIGALRM, writeWait);
        alarm(WRITE_TIME);
        while(true){
            FD_ZERO(&read_fds);
            FD_SET(server_fd, &read_fds);
            for(int i=0 ; i<ctr_client ; i++){
                if(!flag_client[i])
                    continue;
                FD_SET(connect_fd[i], &read_fds);
            }

            void *p = NULL;
            if(flag_block)
                p = &wait_time;

            switch(select(max_fd + 1, &read_fds, &write_fds, NULL, p)){
                case -1:    // alarm ring to arouse the -1 return of select function
                    if(errno != EINTR)
                        myExit();

                case 0: break;
                default:
                    if(FD_ISSET(server_fd, &read_fds))
                        createNewConnect();
                    for(int i=0 ; i < ctr_client; i++){
                        if(FD_ISSET(clientInfo[i].cfd, &read_fds) && clinetInfo[i].count % 2){
                            sRecv(clinetInfo[i].count, clientInfo[i].cfd, &clientInfo[i].msg);
                            FD_SET(clientInfo[i].cfd, &write_fds);  // TODO
                        }
                    }

                    for(int i=0 ; i < ctr_client; i++){
                        if(FD_ISSET(clientInfo[i].cfd, &write_fds) && clinetInfo[i].count % 2 == 0){
                            sSend((clinetInfo[i].count, clientInfo[i].cfd);
                        }
                    }

            }
        }
    }

    close(server_fd);
    return 0;
}