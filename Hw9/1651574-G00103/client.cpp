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

bool dealFork();

void sigCounter(int num)
{
    int status;
    int pid;
    while((pid=waitpid(-1,&status,WNOHANG)) > 0){
		
        run_child--;
		cout <<"sigCounter run child "<<run_child<<endl;
    }
}

void sigCntEnd(int num)
{
    int status;
    int pid;
    while((pid=waitpid(-1,&status,WNOHANG)) > 0){
		//cout << "sigCntEnd"<<endl;
        cnt_ended++;
		run_child -- ;
		cout <<"sigCntEnd  run child "<<run_child<<" cnt_ended "<<cnt_ended<<endl;

    }
}

void sigReplace(int num)
{
    int status;
    int pid;
    while((pid=waitpid(-1,&status,WNOHANG)) > 0){
		cout <<"sigReplace"<<endl;

        dealFork();
    }
}

void clientInit()
{
	
}

int createCfd ()
{
	int cfd =  socket(AF_INET, SOCK_STREAM, 0);
	if(cfd < 0)
	{
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
	struct timeval timeout = {5,0};

	struct sockaddr * addr = (struct sockaddr *) &server_addr;
	if(connect(fp , addr , sizeof(struct sockaddr))<0){
		if(errno==EINPROGRESS){
			printf("connecting ...\n");

			fd_set writeset ;
			FD_ZERO(&writeset);
			FD_SET(fp,&writeset);
			if(select(fp+1, NULL,&writeset,NULL,&timeout)>0){
				if(FD_ISSET(fp,&writeset)){
					return 1 ;
				}
			}	
		}

	}
	else
		return 1 ;
	printf("connect error ! \n");
	return -1 ;
}

int selectNewClientinfo(ClientInfo * cinfos , int len )
{
	int i;
	for (i = 0; i < len; i++)
	{
		if (cinfos[i].flag == DONE || cinfos[i].flag == DEAD)
			return i;
	}
	return -1 ;
}

// 激活 client
bool actiClient( int cfd,  ClientInfo * cinfos , int len )
{
	int i = selectNewClientinfo(cinfos, len );
	max_clientinfo = max_clientinfo > i ? max_clientinfo : i ;
	printf("i=%d max :%d\n",i,max_clientinfo);
	if(i<0)
		return false ;
	cinfos[i].cfd = cfd;
	cinfos[i].flag = ALIVE;
	cinfos[i].count = 0;
	cinfos[i].rwLen = 0;
	FD_SET(cfd , &read_fds);
	FD_SET(cfd , &write_fds);
	return true ;
}

void killClient(ClientInfo &cinfo)
{
	if(cinfo.count>=8)
	{
		if(writeFile(cinfo, false)==false)
		{
			cinfo.flag = WAITWRITE;
			return ;
		}
		cinfo.flag = DONE;
		if(!flag_fork)
			cnt_ended ++ ;
		//cout<<"ready write \n";
	}
	else
		cinfo.flag = DEAD;

	close(cinfo.cfd);
	cout <<"cfd close !\n";
	FD_CLR(cinfo.cfd, &read_fds);
	FD_CLR(cinfo.cfd, &write_fds);
	/*
	if (cinfo.msg.randomMsgLen && cinfo.msg.randomMsg)
	{
		free(cinfo.msg.randomMsg);
		//delete cinfo.msg.randomMsg;
		cinfo.msg.randomMsg = NULL;
	}
	*/
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
	if(sndNum <0)
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
			
			cout << "recv end \n";
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

	while (run_child > 20)
	{
		sleep(1);
	}
	int pid = fork();
	if (pid < 0)
	{
		cerr << "fork failed !\n";
		return false ;
	}
	if (pid > 0)
		return true ;


	prctl(PR_SET_PDEATHSIG,SIGKILL);

	int	cfd = createCfd();
	while(cfd <0)
		cfd = createCfd();

	while(createNewConnect(cfd)<0)
		sleep(1);

	int ppid = getppid();
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
			if(cinfo.count%2== 0)
			{
				if (cRecv(cinfo) == false)
				{
					kill(ppid, SIGREPLACE);
					exit(0);
				}
			}
			if(cinfo.count%2)
			{
				if (cSend(cinfo) == false)
				{
					kill(ppid, SIGREPLACE);
					exit(0);
				}
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
	kill(ppid,SIGCHILDDONE);
	exit(0);

}

int main(int argc , char *argv[])
{
	srand(unsigned(time(0)));
	parseCMD(argc , argv , true);


	printf("flag_block =%d , flag_fork=%d , num =%d \n",flag_block,flag_fork,max_num);

	if(flag_fork)
	{
		signal(SIGCHILDDONE,sigCntEnd);
		signal(SIGREPLACE,sigReplace);
		signal(SIGCHLD, sigCounter);
		for (int i=0;i<max_num;i++){
			while(run_child >= 100)
				sleep(1);
			run_child ++ ;
			
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
		ClientInfo * clientinfos = new ClientInfo[CLIENT_MAX_CONNECT];
		memset(clientinfos,0,sizeof(clientinfos));
		/*
				for (int i =0 ; i < max_num;i++)
		{
			cfd[i]=createCfd();
			if(cfd[i]<0)
			{
				cerr<<"cfd "<<i<<"failed "<<endl;
				i--;
			}
		}
		*/

		for(int i =0 ; i<max_num;i++)
		{
			int cfd = createCfd();
			//cout<<i<<' '<<cfd<<endl;
			if(cfd <0)
			{
				cerr<<"cfd "<<i<<"failed "<<endl;
				i--;
			}
			while(createNewConnect(cfd)<0)
			{
				cerr<<"cfd "<<i<<"connect" <<"error "<<endl ;
				//sleep(1);
			}
			while(actiClient(cfd,clientinfos,CLIENT_MAX_CONNECT)==false )
			{
				cerr<<"cfd "<<i<<"connect" <<"error "<<endl ;
				//sleep(1);
			}
		}

		fd_set rfd_cpy , wfd_cpy ;
		timeval wait_time;
		while(cnt_ended < max_num)
		{
			rfd_cpy = read_fds;
			wfd_cpy = write_fds;
			setTime(wait_time,1);
			switch(select(max_fd+1,&rfd_cpy,&wfd_cpy,NULL,&wait_time))
			{
				case -1 :
					cerr<<"select error !\n";
					break;
				case 0:
					break;
				default :
					//cout<<"select success !\n";
					int newmax = 0;
					for (int i = 0; i < max_clientinfo + 1; i++)
					{
						printf("i= %d flag = %d \n",i,clientinfos[i].flag);
						//// 重新连接
						if(clientinfos[i].flag == DEAD)
						{
							int cfd = createCfd();
							if (cfd < 0)
							{
								
								cerr << "cfd " << i << "failed " << endl;
								i--;
							}
							while (createNewConnect(cfd) < 0)
							{
								cerr << "cfd " << i << "connect"
									 << "error " << endl;
								//sleep(1);
							}
							clientinfos[i].cfd = cfd;
							clientinfos[i].flag = ALIVE;
							clientinfos[i].count = 0;
							clientinfos[i].rwLen = 0;
							FD_SET(cfd , &read_fds);
							FD_SET(cfd , &write_fds);
							
							continue;
						}

						if (clientinfos[i].flag != ALIVE)
							continue;

						//printf("i=%d max=%d\n",i,max_clientinfo);
						//cout<<"count "<<clientinfos[i].count<<endl;
						newmax = i;

						if (FD_ISSET(clientinfos[i].cfd, &rfd_cpy))
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
						if (clientinfos[i].count % 2 && FD_ISSET(clientinfos[i].cfd, &wfd_cpy))
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



	return 0;
}
