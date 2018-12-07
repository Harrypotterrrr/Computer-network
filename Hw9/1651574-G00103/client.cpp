#include "myutil.h"
#include <sys/wait.h>
#include <sys/prctl.h>
#define RANDOMMSGLANMAX 99999
#define CLIENT_MAX_CONNECT 1024

#define SIGCHILDDONE 45
#define SIGREPLACE 46

const static int stu_id=1652237;
const static int SIZE = 64 ;
fd_set read_fds, write_fds;
int max_clientinfo , max_fd ;
int cnt_ended = 0;
int run_child = 0;

int totalConnectCfd =0;


bool dealFork();

void sigCounter(int num)
{
    int status;
    int pid;
    while((pid=waitpid(-1,&status,WNOHANG)) > 0){
		
        //run_child--;
		cout <<"sigCounter run child "<<run_child<<endl;
    }
}

void sigCntEnd(int num)
{
	cnt_ended++;
	run_child--;
	cout << "sigCntEnd  run child " << run_child << " cnt_ended " << cnt_ended << endl;
}

void sigReplace(int num)
{
	run_child -- ;
 	cout <<"sigReplace"<<endl;
    dealFork();
}

void clientInit()
{
	
}

int createCfd ()
{
	int cfd =  socket(AF_INET, SOCK_STREAM, 0);
	if(cfd < 0 ||cfd >1023)
	{
		if(cfd >1023)
			close(cfd);
		cerr<<"cfd socoket failed !\n";
		return -1 ;
	}
	if(!flag_block)
		setNonBlock(cfd);
	max_fd = max_fd > cfd ? max_fd :cfd ;
	return cfd ;
}


int createNewConnect(int fp )
{	
	struct timeval timeout = {1,0};

	struct sockaddr * addr = (struct sockaddr *) &server_addr;
	if (connect(fp, addr, sizeof(struct sockaddr)) < 0)
	{
		if (errno == EINPROGRESS)
		{
			//printf("connecting ...\n");

			fd_set writeset ,readset;
			FD_ZERO(&writeset);
			FD_SET(fp, &writeset);
			readset = writeset ; 
			if (select(fp + 1, &readset, &writeset, NULL, &timeout) > 0)
			{

				if (FD_ISSET(fp, &writeset) || FD_ISSET(fp, &readset))
				{
					int error;
					socklen_t len = sizeof(error);
					if (getsockopt(fp, SOL_SOCKET, SO_ERROR, &error, &len) == -1)
					{
						//break;
						printf("getsockopt return -1.\n");
					}
					else if (error != 0)
					{
						//break;
						printf("getsockopt return errinfo = %d.\n", error);
					}
					else
					{
						if(flag_block)
							setNonBlock(fp);
						//printf("connect success\n");
						return 0;
					}
				}

/* 				if (FD_ISSET(fp, &writeset))
				{
					socklen_t peerlen =0;
					getpeername(fp,addr ,&peerlen);

					if(peerlen>0)
					{
						if (!flag_block)
							setNonBlock(fp);
						//cout << "my conn ok!\n";
						return 1;
					}

				} */
			}
		}
	}
	else
	{
		if (!flag_block)
			setNonBlock(fp);
		return 1;
	}

	//printf("create new connect error ! %d %s\n",errno , strerror(errno));
	return -1 ;
}

int selectNewClientinfo(ClientInfo * cinfos , int len )
{
	static int i =0 ;
	int j = 0 ;
	while(j<len)
	{
		if (cinfos[i].flag == DONE || cinfos[i].flag == DEAD)
			return i;

		if(++i>=len)
			i=len ; 
		j++;
	}

	return -1 ;
}

// 激�?? client
//bool actiClient( int cfd,  ClientInfo * cinfos , int len )
bool actiClient(  ClientInfo * cinfos , int i)
{
	//int i = selectNewClientinfo(cinfos, len );
	max_clientinfo = max_clientinfo > i ? max_clientinfo : i ;
	//printf("i=%d max :%d\n",i,max_clientinfo);
	if(i<0){
		cerr<<"actiClient failed !\n";
		return false ;
	}
	//cinfos[i].cfd = cfd;
	cinfos[i].flag = ALIVE;
	cinfos[i].count = 0;
	cinfos[i].rwLen = 0;
	FD_SET(cinfos[i].cfd , &read_fds);
	FD_SET(cinfos[i].cfd , &write_fds);
	return true ;
}

void killClient(ClientInfo &cinfo)
{
	if(cinfo.flag==DONE)
		return ;
	if(cinfo.flag == ALIVE&&cinfo.count>8)
	{
		if(writeFile(cinfo, false)==false)
		{
			//cinfo.flag = WAITWRITE;
			return ;
		}
		cinfo.flag = DONE;
		if(!flag_fork)
			cnt_ended ++ ;

		cout <<"wirte ok  num="<<cnt_ended<<endl;
		//cout<<"ready write \n";
	}
	else
		cinfo.flag = DEAD;


	////////// UNDO///////////

	if(flag_fork)
		close(cinfo.cfd);
	else if(cinfo.count>8)
		//close(cinfo.cfd);
		shutdown(cinfo.cfd,2);
	else
		shutdown(cinfo.cfd,2);
	//cout <<"cfd close !\n";
	FD_CLR(cinfo.cfd, &read_fds);
	FD_CLR(cinfo.cfd, &write_fds);

	return;
}

bool cSend(ClientInfo & cinfo)
{

	//static char sndbuff [SIZE+4];

	int count = cinfo.count/2 ;
	int sndNum ;
	int sndLen ;

	switch(count )
	{
		case GETSTUNO:
		{
			sndLen = sizeof(int );
			int data = htonl(cinfo.msg.StuNo);
			sndNum = send(cinfo.cfd,&data + cinfo.rwLen ,sndLen-cinfo.rwLen ,0);
			break;
		}
		case GETPID:
		{
			sndLen = sizeof(int);
			int data = htonl(cinfo.msg.clientPid);
			sndNum = send(cinfo.cfd,&data + cinfo.rwLen ,sndLen-cinfo.rwLen ,0);		
			break;
		}
		case GETTIME:
		{
			sndLen = 19;
			sndNum = send(cinfo.cfd,&cinfo.msg.clientTime + cinfo.rwLen ,sndLen-cinfo.rwLen ,0);
			break;
		}
		case GETSTR:
		{
			sndLen = cinfo.msg.randomMsgLen;
			sndNum = send(cinfo.cfd,&cinfo.msg.randomMsg + cinfo.rwLen ,sndLen-cinfo.rwLen ,0);
			break;
		}

	}
	if(sndNum <=0)
	{
		cerr<<"client "<<cinfo.cfd<<" send failed !\n";
		killClient(cinfo);
		return false ;
	}
	cinfo.rwLen += sndNum ;
	if(cinfo.rwLen == sndLen )
	{
		cinfo.count++;
		cinfo.rwLen = 0;
		//cout<<"client "<<cinfo.cfd<<"count "<<cinfo.count<<"send ok !\n";
	}
	else if (cinfo.rwLen > sndLen  )
	{
		cerr<<"client "<<cinfo.cfd<<" send failed !\n";
		cout << cinfo.rwLen << ":"<<sndLen<<endl;
		killClient (cinfo);
		return false ;
	}

	return true;

}

bool cRecv(ClientInfo & cinfo)
{
	static char msgbuff[SIZE+4];
	int count = cinfo.count/2;

	int readNum = recv(cinfo.cfd,msgbuff,SIZE,0);
	bool flag = true ;


	static int icount =0 ;
	switch( count )
	{
		case  GETSTUNO:
		{
			if(readNum!=5||strncmp(msgbuff,"StuNo",5))
				flag = false ;
			cinfo.msg.StuNo = stu_id;
			break;
		}
		case GETPID :
		{
			if(readNum!=3||strncmp(msgbuff,"pid",3))
				flag = false ;
			if(flag_fork)
				cinfo.msg.clientPid = getpid();
			else	
				cinfo.msg.clientPid = (getpid()<<16)+cinfo.cfd;
			break;
		}
		case GETTIME :
		{
			if(readNum!=5|| strncmp(msgbuff,"TIME\0",5))
				flag = false ;
			time_t t = time(0);
            strftime(cinfo.msg.clientTime, sizeof(cinfo.msg.clientTime), "%Y-%m-%d %X\0",localtime(&t));
			break;
		}
		case GETSTR :
		{
			if(readNum!=9 || strncmp(msgbuff,"str",3))
				flag = false ;
			cinfo.msg.randomMsgLen =  atoi(msgbuff+3);
			for (int i =0;i<cinfo.msg.randomMsgLen ;i++){
				cinfo.msg.randomMsg[i]= rand()%256;
				//cout<<"msg "<<unsigned(cinfo.msg.randomMsg[i])<<endl;
			}
			break;
		}
		case END :
		{
			if(readNum!=3 || strncmp(msgbuff,"end",3))
			{
				flag = false ;
				cinfo.count = 0 ;
			}
			if (flag)
				cout << "end i = " << ++icount << " d = " << ++cinfo.count << endl;

			killClient(cinfo);
			
			return flag ;
		}
	}

	if(flag == false )
	{
		cout<<"client "<<cinfo.cfd<<" recv error !\n";
		killClient(cinfo);
		return false ;
	}

	cinfo.count++;
	cinfo.rwLen = 0;
	//cout<<"client "<<cinfo.cfd<<"count "<<cinfo.count<<" recv ok !\n";

	
	return true ;
}

bool dealFork(  )
{
	int pid = fork();
	if (pid < 0)
	{
		cerr << "fork failed !\n";
		return false ;
	}
	if (pid > 0)
	{
		run_child++;
		return true;
	}

	prctl(PR_SET_PDEATHSIG,SIGKILL);

	int	cfd = createCfd();
	//while(cfd <0)
	//	cfd = createCfd();

	int ppid = getppid();

	if (cfd < 0 || createNewConnect(cfd) < 0)
	{
		cout << "fork cfd error !\n";
		kill(ppid, SIGREPLACE);
		exit(0);
	}

	ClientInfo cinfo ;

	cinfo.cfd = cfd;
	cinfo.count = 0;
	cinfo.flag = ALIVE;
	cinfo.rwLen = 0;
	memset(&(cinfo.msg), 0, sizeof(cinfo.msg));

	if(flag_block)
	{
		while (cinfo.flag!= DONE )
		{

			if(cinfo.flag == DEAD)
			{
				close(cfd);
				kill(ppid,SIGREPLACE);
				exit(0);
			}

			if(testConnect (cfd )== false)
			{
				killClient(cinfo);
				continue;
			}

			if(cinfo.count%2== 0)
			{
				cRecv (cinfo);

			}
			if(cinfo.count%2)
			{
				cSend(cinfo);
			}
				
		}
	}
	else
	{
		//cout <<"unblock !\n";
		FD_ZERO(&read_fds);
		FD_SET(cfd, &read_fds);
		write_fds = read_fds;
		fd_set rfd_cpy, wfd_cpy;
		timeval wait_time;

		while(cinfo.flag !=DONE)
		{
			if(cinfo.flag == DEAD)
			{
				close(cfd);
				kill(ppid,SIGREPLACE);
				exit(0);
			}
			setTime(wait_time,1);
			rfd_cpy = read_fds;
			wfd_cpy = write_fds;
			switch (select(cfd + 1, &rfd_cpy, &wfd_cpy, NULL, &wait_time))
			{
			case -1:
				cerr<<"select error !\n";
				break;
			case 0 :
				//cerr<<"fork select timeout !\n";
				break;
			default :

				if(FD_ISSET(cfd,&rfd_cpy))
				{
					if(testConnect(cfd)==false)
					{
						killClient(cinfo);
						break;
					}
					if(cinfo.count%2==0)
						cRecv(cinfo);
				}
				if(cinfo.count%2&&FD_ISSET(cfd,&wfd_cpy))
					cSend(cinfo);
				break;
			}
		}
	}
	cout <<"client "<<cinfo.cfd<<"done "<<ppid <<endl;
	close(cfd);
	kill(ppid,SIGCHILDDONE);
	exit(0);

}

bool reConnect ( ClientInfo & cinfo)
{
	
	int &cfd = cinfo.cfd;
	close (cfd);
	cfd = createCfd();
	cout <<"new cfd ="<<cinfo.cfd<<endl;
	if (createNewConnect(cfd) < 0)
	{
		cerr << "reconnect cfd " <<cfd << "connect " << "error " << endl;
		return false;
	}
	cinfo.flag = ALIVE;
	cinfo.count = 0;
	cinfo.rwLen = 0;
	FD_SET(cfd, &read_fds);
	FD_SET(cfd, &write_fds);
	return true;
}

int makeBatchConn(ClientInfo * cinfos , int len )
{
	static int connNum = 0;
	int i;
	static bool flag = true;
	if(flag)
	{
		for (i=0;i<max_num ;i++)
		{
			cinfos[i].cfd = createCfd();
			if(cinfos[i].cfd<0)
				i--;
		}
		flag = false;
		cout<<"batch conn ok !\n";
	}

	for ( i = connNum; i < max_num; i++)
	{
		//int cfd = createCfd();
		//cout<<i<<' '<<cfd<<endl;
/* 		if (cfd < 0)
		{
			cerr << "cfd " << i << "failed " << endl;
			//i--;
			break ;
		} */
		if (createNewConnect(cinfos[i].cfd) < 0)
		{
			//cerr << "cfd " << cinfos[i].cfd << "connect "<< "error " << endl;
			//close(cfd);
			break;
		}
		if (actiClient(cinfos, i) == false)
		{
			cerr << "cfd " << cinfos[i].cfd << "actiClient" << " error " << endl;
			//close(cfd);
			break;
		}
	}
	connNum = i;
	//cout <<"connNUm ="<<connNum <<endl;
	return connNum ;
}


int main(int argc , char *argv[])
{
	create_daemon();
	srand(unsigned(time(0)));
	parseCMD(argc , argv , true);
	signal(SIGPIPE,SIG_IGN);

	printf("flag_block =%d , flag_fork=%d , num =%d \n",flag_block,flag_fork,max_num);

	if(flag_fork)
	{
		signal(SIGCHILDDONE,sigCntEnd);
		signal(SIGREPLACE,sigReplace);
		signal(SIGCHLD, SIG_IGN);
		for (int i=0;i<max_num;i++){
			//while(run_child >= 100)
			//	sleep(1);
			
			if(dealFork()==false)
				i--;
		}
		cout <<" fork done ,wait end !\n";

		while(cnt_ended<max_num)
			sleep(1);
	}
	else 
	{
		//int cfd[CLIENT_MAX_CONNECT];
		ClientInfo * clientinfos = new (nothrow)ClientInfo[CLIENT_MAX_CONNECT+10];
		if(clientinfos == NULL){
			cerr <<"new clientinfos failed !\n";
			exit(0);
		}
		memset(clientinfos,0,sizeof(clientinfos));

		fd_set rfd_cpy , wfd_cpy ;
		timeval wait_time;
		int icount = 0;
		int batchConn = 0;
		while(cnt_ended < max_num)
		{
			if(batchConn <max_num&&icount++%8==0)
				batchConn =makeBatchConn(clientinfos , CLIENT_MAX_CONNECT);
			rfd_cpy = read_fds;
			wfd_cpy = write_fds;
			setTime(wait_time,0,100);
			switch(select(max_fd+1,&rfd_cpy,&wfd_cpy,NULL,&wait_time))
			{
				case -1 :
					cerr<<"select error !\n";
					break;
				case 0:
					break;
				default :
					//cout <<"cnt_ended "<<cnt_ended<<endl;
					//cout<<"select success !\n";
					int newmax = 0;
					for (int i = 0; i < 1024; i++)
					{
						//printf("i= %d flag = %d \n",i,clientinfos[i].flag);
						//// 重新连接
						if(clientinfos[i].flag == DEAD)
						{
							cout <<"cfd"<<clientinfos[i].cfd<<" reconnect !\n";
							reConnect(clientinfos[i]);
							newmax = i;
							//sleep(1);
						}

						newmax = i;
						if (clientinfos[i].flag == ALIVE&&FD_ISSET(clientinfos[i].cfd, &rfd_cpy))
						{
							if (testConnect(clientinfos[i].cfd) == false)
							{
								killClient(clientinfos[i]);
								continue;
							}
							//cout<<"max_fd"<<max_fd<< "select cfd ="<<clientinfos[i].cfd <<endl ;
							if (clientinfos[i].count % 2 == 0)
								cRecv(clientinfos[i]);
						}


/*						if (FD_ISSET(clientinfos[i].cfd, &rfd_cpy))
						{
							if (testConnect(clientinfos[i].cfd) == false)
							{
								killClient(clientinfos[i]);
								continue;
							}
							//cout<<"max_fd"<<max_fd<< "select cfd ="<<clientinfos[i].cfd <<endl ;
							if (clientinfos[i].count % 2 == 0)
								cRecv(clientinfos[i]);
						}*/

						if (clientinfos[i].flag == ALIVE&&clientinfos[i].count % 2 && FD_ISSET(clientinfos[i].cfd, &wfd_cpy))
						{
							cSend(clientinfos[i]);
						}

					}
					max_clientinfo = newmax ;
					break;
			}
		}
		
		delete clientinfos;

	}

	for (int i = 2 ;i<1024;i++)
		close(i);


	return 0;
}
