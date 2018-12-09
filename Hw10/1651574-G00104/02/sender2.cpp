#include "../common/common.h"


#define transDataName "send2_data"
const char *pidFileName = "send2pid";



// 设置缓冲区域大小
const int buffSize = 1 ;


void send2_network()
{

	while(PL_Connect==false)
		sleep(1);
	cout <<"network conn ok !\n";
	getLayerPid(nt_pid,dl_pid,ps_pid,pidFileName);
	cout <<"network read pid ok "<<endl;
	unlink(pidFileName);
	connectFifo();


	ifstream fin ;
	fin.open(transDataName,ios::in|ios::binary);
	if(fin.is_open()==false)
	{
		cerr<<"打开文件失败";
		exit(0);
	}
	cout <<"nums of net "<< nNet_to_dl <<endl ;

	Packet p ;
	bool flag =true; 

	while(true)
	{
		//  如果缓存满 则入睡
		while(nNet_to_dl == buffSize)
			sleep(1);
		nNet_to_dl ++ ;

		memset(p.data, 0, sizeof(p));
		if (flag)
			flag = fin.read((char *)&p, sizeof(p));

		network_to_datalink(p,1);

		kill(dl_pid,SIG_NETWORK_TO_DATALK);

		cout <<"send1 net to dt link ok !\n";
		if(flag == 0)
		{
			if(allZero(p)==false)
			{
				cout <<"still remain "<<endl;
				continue;
			}
			kill(dl_pid,SIG_DONE);
			cout<<"send1 network layer done \n";
			fin.close();
			return ;
		}

		
	}
}

void send2_dtlink()
{
	Frame s ;
	Packet buffer ;
	while(PL_Connect==false)  
		sleep(1);

	cout <<"dtlink conn ok !\n";
	getLayerPid(nt_pid,dl_pid,ps_pid,pidFileName);
	cout <<"dtlink getpid ok !\n";
	kill(nt_pid ,SIG_PL_CONNECT);

	connectFifo();

	event_type event ;
	while(true)
	{
		while(nNet_to_dl==0)
			sleep(1);
		cout <<"dt link read ok !\n";
		nNet_to_dl-- ;

		datalink_from_network(s.info,1);
		s.kind = htonl(data);
		datalink_to_physical(s);
		kill(nt_pid,SIG_DATALK_FROM_NETWORK);
		cout <<ps_pid<<endl;

		//用到了  SIG_DATALK_TO_PHYSIC信号
		kill(ps_pid,SIG_DATALK_TO_PHYSIC);
		cout <<"kill ok !\n";

		cout <<"frame size " << frameSize(s)<<"!!!!!!!!!!!!"<<endl; 
		wait_for_event(event); 
		datalink_from_physical(s); 
		cout <<"wait ok !\n"; 
	} 
	exit(0);

}

void send2_physic(int argc , char *argv[])
{

	struct sockaddr_in server_addr; 
	int port =atoi(argv[2]);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port= htons(port);
	server_addr.sin_family=AF_INET;

	int cfd ; 
	if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		myExit();

	if(connect(cfd,(struct sockaddr *)&server_addr,sizeof(sockaddr))==-1)
	{
		cerr<<"connect error !\n";
		cout<<nt_pid<<' '<<dl_pid<<" "<<ps_pid<<endl;
		exit(0);
	}
	setNonBlock(cfd); 

	cout<<"cfd = "<<cfd<<endl;
	ps_pid = getpid();
	PL_Connect = true ;
	kill(dl_pid ,SIG_PL_CONNECT);

	connectFifo();

	Frame s ;
	Frame s_recv ; 

	fd_set fdset ;
	FD_ZERO(&fdset);
	FD_SET(cfd,&fdset);
	timeval timeout ;
	fd_set rset_cpy , wset_cpy ;
	FD_ZERO(&rset_cpy);
	FD_ZERO(&wset_cpy);
	setTime(timeout,0,40);

	int dataLen ;

	while(true)
	{
		rset_cpy = fdset;
		setTime(timeout,1,10);
		if(nDl_to_phy>0)  //可写
			wset_cpy = fdset ;  
		else
			FD_ZERO(&wset_cpy);
		switch(select(cfd+1,&rset_cpy,&wset_cpy,NULL,&timeout))
		{
			case -1 :
				cout <<"select err!\n";
				break ;
			case 0:
				cout <<"time out "<<endl;
				break;
			default:
			if(FD_ISSET(cfd,&rset_cpy))
			{
				// 先不考虑读不满的情况
				cout <<"start read !\n";
				dataLen = sizeof(s_recv);
				int recvNum = recv(cfd, &s_recv,dataLen, 0);
				if (recvNum <= 0)
				{
					cout << "send done " << endl;
					kill(getppid(),60);
					exit(0);
				}
				else
				{
					cout <<"recv ok ! "<<recvNum<<endl;
					physical_to_datalink(s_recv);
					kill(dl_pid , SIG_FRAME_ARRIVAL);
				}
			}
			if(FD_ISSET(cfd,&wset_cpy))
			{
				cout <<"start write !\n";
				if(physical_layer_from_datalink(s)==FALSE)
				{
					cerr<<"from physical failed !\n";
					exit(0);
					break;
				}

				dataLen = sizeof (s);
				//dataLen = frameSize(s);
				cout <<"dataLen "<< frameSize(s)<<endl;
				int sndNum = send(cfd, &s, dataLen , 0);
				if (sndNum > 0){
					cout<<"snd ok ！"<<sndNum<<endl;
					nDl_to_phy--;
				}
				else
				{
					cerr << "send failed !\n";
					exit(0);
					break;
				}

			}
		}

	}
	cout<<"send2_physic ok !\n";
	exit(0);
}

void fork_layer(int argc , char *argv [])
{

	pid_t pid ;
	Init();


	ofstream fout (pidFileName,ios::out|ios::trunc);

	// 1.网络层 2.数据链路层 3.物理层

	for(int i=0;i<3;i++)
	{
		pid = fork();
		if(pid <0)
		{
			cerr <<"fork error !\n";
			return ;
		}
		if(pid >0)
		{
			fout<<pid <<endl;
			if(i==0)
				nt_pid=pid;
			else if (i==1)
				dl_pid=pid;
			else if (i==2)
			{
				ps_pid=pid ;
				fout.close();
			}
		}
		else
		{
			prctl(PR_SET_PDEATHSIG,SIGKILL);
			if(i==0)
				send2_network();
			else if (i==1)
				send2_dtlink();
			else if (i==2)
				send2_physic(argc , argv );
			//cout <<i<<" "<<"23333"<<endl;
		}
	}
	cout <<" fork layers ok !\n";
	return ;
}

int main(int argc , char *argv[])
{
	signal(SIGCHLD,SIG_IGN);
	create_daemon();
	sig_catch();
	fork_layer(argc, argv);

	while(1)
		sleep(1);

}
