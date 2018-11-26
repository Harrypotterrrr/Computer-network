#include <fstream>
#include <sys/prctl.h>
#include "server.h"

int ctr_clinet = 0;
int max_fd, server_fd;
int max_clientinfo;
//ClientInfo clientinfos[SERVER_MAX_CONNECT];
ClientInfo * clientinfos ; 
fd_set read_fds, write_fds;

using namespace std;

void serverInit()
{
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		myExit();

	max_fd = server_fd;
	//memset(clientinfos, 0, sizeof(clientinfos));
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_SET(server_fd, &read_fds);

	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		myExit();

	if (listen(server_fd, MAX_LISTEN)==-1)
		myExit();

	if (!flag_block)
		setNonBlock(server_fd);

	setReusePort(server_fd );

	return;
}

int selectNewClientinfo()
{
	int i;
	for (i = 0; i < SERVER_MAX_CONNECT; i++)
	{
		if (clientinfos[i].flag == DONE || clientinfos[i].flag == DEAD)
			return i;
	}
	return -1 ;
}

bool  createNewConnect()
{
	struct sockaddr_in client_addr;
	socklen_t len_client_addr = sizeof(client_addr);

	int i = selectNewClientinfo();
	if (i < 0)
	{
		cerr << "no clientinfo , please wait !\n";
		return false;
	}

	int cfd = accept(server_fd, (struct sockaddr *)&client_addr, &len_client_addr);
	if (cfd < 0)
	{
		cerr << "accept failed !\n";
		return false ;
	}

	clientinfos[i].cfd = cfd;
	clientinfos[i].flag = ALIVE;
	clientinfos[i].count = 0;
	clientinfos[i].rwLen = 0;
	memset(&(clientinfos[i].msg), 0, sizeof(clientinfos[i].msg));

	if (1)
	{
		cout << "connect ok , i= " << i << endl;
	}

	FD_SET(cfd, &read_fds);
	FD_SET(cfd, &write_fds);

	if (i > max_clientinfo)
		max_clientinfo = i;

	max_fd = max_fd > cfd ? max_fd : cfd;

	if (!flag_block)
		setNonBlock(cfd);

	return true ;
}

void killClient(ClientInfo &cinfo)
{
	if(cinfo.count>8)
	{
		if(writeFile(cinfo, true)==false)
			return ;
		//cout<<"ready write \n";
	}
	cinfo.flag = DEAD;
	close(cinfo.cfd);
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


bool sSend(ClientInfo &cinfo)
{
	static char strMsg[10];
	int count = cinfo.count / 2;

	const char *sndMsg;

	if (count == GETSTR)
	{
		if (cinfo.msg.randomMsgLen == 0)
		{
			cinfo.msg.randomMsgLen = 32768 + ((rand() + 1e-6) / RAND_MAX * 67231);
			/*
			cinfo.msg.randomMsg = (char *)malloc(cinfo.msg.randomMsgLen * sizeof(char));
			if (cinfo.msg.randomMsg == NULL)
			{
				cerr << "malloc error , please wait \n";
				cinfo.msg.randomMsgLen = 0;
				return false;
			}
			*/
		}

		sprintf(strMsg, "str%d", cinfo.msg.randomMsgLen);
		sndMsg = strMsg;
	}
	else
		sndMsg = orderMsg[count];

	int MsgLen = strlen(sndMsg);

	if(count==GETTIME||count==GETSTR)
		MsgLen ++;

	int sndNum = send(cinfo.cfd, sndMsg + cinfo.rwLen, MsgLen - cinfo.rwLen, 0);

	if (sndNum < 0)
	{
		// TODO
		cerr << "cllient_cfd :" << cinfo.cfd << "snd error !\n";
		killClient(cinfo);
		return false;
	}

	cinfo.rwLen += sndNum;

	if (cinfo.rwLen == MsgLen)
	{
		cinfo.count++;
		cinfo.rwLen = 0;
	}
	else if (cinfo.rwLen >MsgLen)
	{
		killClient(cinfo);
		return false ;
	}

	//printf("send success , cfd = %d,count = %d \n", cinfo.cfd,cinfo.count);
	return true;
}

bool sRecv(ClientInfo &cinfo)
{
	
	if (cinfo.count > 8)
	{
		cout << "client " << cinfo.cfd << "ok !\n";
		cinfo.flag = DONE ;
		killClient(cinfo);
		return true ;
	}


	int count = cinfo.count / 2;
	int MsgLen;
	int readNum;

	switch (count)
	{
	case GETSTUNO:
		MsgLen = sizeof(int);
		readNum = recv(cinfo.cfd, &(cinfo.msg.StuNo) + cinfo.rwLen, MsgLen, 0);
		break;
	case GETPID:
		MsgLen = sizeof(int);
		readNum = recv(cinfo.cfd, &(cinfo.msg.clientPid) + cinfo.rwLen, MsgLen, 0);
		break;
	case GETTIME:
		MsgLen = 19;
		readNum = recv(cinfo.cfd, (cinfo.msg.clientTime) + cinfo.rwLen, MsgLen, 0);
		break;
	case GETSTR:
		MsgLen = cinfo.msg.randomMsgLen;
		//readNum = recv(cinfo.cfd,(cinfo.msg.randomMsg)+cinfo.rwLen,MsgLen,)
		readNum = recv(cinfo.cfd, (cinfo.msg.randomMsg) + cinfo.rwLen, MsgLen, 0);
		break;
	}
	if (readNum <= 0)
	{ // TODO
		cerr << "cllient_cfd :" << cinfo.cfd << "recv error !\n";
		killClient(cinfo);
		return false;
	}

	cinfo.rwLen += readNum;
	if (cinfo.rwLen == MsgLen)
	{
		if(count==GETSTUNO)
			cinfo.msg.StuNo = ntohl(cinfo.msg.StuNo);
		else if(count == GETPID)
			cinfo.msg.clientPid= ntohl(cinfo.msg.clientPid);
		else if (count == GETTIME)
			cinfo.msg.clientTime[MsgLen] = 0;
		cinfo.count++;
		cinfo.rwLen = 0;
	}
	else if (cinfo.rwLen > MsgLen)
	{
		killClient(cinfo);
		return false;
	}
	//printf("recv success , cfd = %d,count = %d \n", cinfo.cfd,cinfo.count);
	return true;
}


void dealFork(const int cfd)
{
	int pid = fork();
	if (pid < 0)
	{
		cerr << "fork failed !\n";
		return;
	}
	if (pid > 0)
		return;

	prctl(PR_SET_PDEATHSIG,SIGKILL);

	cout << "233"<<endl ;
	ClientInfo cinfo;
	cinfo.cfd = cfd;
	cinfo.count = 0;
	cinfo.flag = ALIVE;
	cinfo.rwLen = 0;
	memset(&(cinfo.msg), 0, sizeof(cinfo.msg));

	if (flag_block)
	{
		while (cinfo.flag == ALIVE)
		{
			if(cinfo.count%2==0)
			{
				if (sSend(cinfo) == false)
					return;
			}
			if(cinfo.count%2)
			{
				if (sRecv(cinfo) == false)
					return;
			}
				
		}
	}
	else
	{
		cout <<"unblock !\n";
		FD_ZERO(&read_fds);
		FD_SET(cfd, &read_fds);
		write_fds = read_fds;
		fd_set rfd_cpy, wfd_cpy;
		timeval wait_time;
		while(cinfo.flag == ALIVE)
		{
			
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
					if(cinfo.count%2)
						sRecv(cinfo);
				}
				if(cinfo.count%2==0&&FD_ISSET(cfd,&wfd_cpy))
					sSend(cinfo);
				break;
			}
		}
	}

	cout << "fork ok !\n";
	exit(0);
	return;
}



int main(int argc, char *argv[])
{
	parseCMD(argc, argv, false);

	printf("flag_block =%d , flag_fork=%d\n",flag_block,flag_fork);


	serverInit();


	fd_set rfd_cpy, wfd_cpy;
	timeval wait_time;

	char msgTest [5];

	signal(SIGCHLD, SIG_IGN);
	if (flag_fork)
	{
		while (1)
		{
			setTime(wait_time, 1);
			rfd_cpy = read_fds;
			
			switch (select(server_fd + 1, &rfd_cpy, NULL, NULL, &wait_time))
			{
			case -1:
				cerr<<"select error !\n";
				break;
			case 0:
				break;
			default:
				cout<<"select success !\n";
				if (FD_ISSET(server_fd, &rfd_cpy))
				{
					int cfd = accept(server_fd, NULL, NULL);
					cout <<"accept ok !\n";
					if(!flag_block)
						setNonBlock(cfd);
					if (cfd < 0)
					{
						cerr << "accept failed !\n";
						break;
					}
					else
						dealFork(cfd);
					cout<<"connect done \n";
				}
				break;
			}
		}
	}
	else
	{
		cout<<"unfork unblock !\n";
		clientinfos = new ClientInfo[SERVER_MAX_CONNECT];
		while (1)
		{
			setTime(wait_time, 1);
			rfd_cpy = read_fds;
			wfd_cpy = write_fds;

			switch (select(max_fd + 1, &rfd_cpy, &wfd_cpy, NULL, &wait_time))
			{
			case -1:
				if (errno != EINTR)
					myExit();
				break;
			case 0:
				//cerr<<"select time out !\n";
				break;
			default:
				int newmax = 0;
				if (FD_ISSET(server_fd, &rfd_cpy))
				{
					while(createNewConnect()==true)
						;
				}
				for (int i = 0; i < max_clientinfo + 1; i++)
				{
					if (clientinfos[i].flag != ALIVE)
						continue;
					newmax = i;
					if ( FD_ISSET(clientinfos[i].cfd, &rfd_cpy))
					{
						if(testConnect(clientinfos[i].cfd)==false)
						{
							killClient(clientinfos[i]);
							continue;
						}
						//cout<<"max_fd"<<max_fd<< "select cfd ="<<clientinfos[i].cfd <<endl ;
						if(clientinfos[i].count % 2)
							sRecv(clientinfos[i]);
					}
					if (clientinfos[i].count % 2 == 0&&FD_ISSET(clientinfos[i].cfd, &wfd_cpy) )
					{
						sSend(clientinfos[i]);
					}
				}
				max_clientinfo = newmax;
				break;
			}
		}
		delete clientinfos;
	}

	return 0;
}
