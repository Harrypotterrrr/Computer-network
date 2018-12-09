#include "../common/common.h"

#define transDataName "receive2_data"
#define buffername "send1_network_buffer"
const char *pidFileName = "receive1pid";


// 链路层到网络层的个数
//int ndl_to_net = 0 ;
//int nphy_todl = 0;

void myLog(const char *msg , const Frame & r )
{

	ofstream fout ("log",ios::out|ios::app);
	fout <<msg<< " seq ="<<hex<<r.seq<<" ack = "<<dec<<r.ack<<endl;
	fout.close();
	return ;
}

bool randProb(double x )
{
	double p = rand()/ (RAND_MAX+1e-6);
	if(p<x)
		return true;
	return false ;
}

void mySleep( int s ,int us )
{
	timeval timeout = {s,us};
	select(10,NULL,NULL,NULL,&timeout);
	return ;
}

void receive2_network()
{
	
	while(PL_Connect==false)
		sleep(1);
	
	//获取所有的pid 
	getLayerPid(nt_pid,dl_pid,ps_pid,pidFileName);
	unlink(pidFileName);
	//fifo_dton = open(FIFO_TO_NETWORK,O_RDONLY);


	connectFifo();
	cout <<"network read pid ok "<<endl;

	ofstream fout ;
	fout.open(transDataName,ios::out|ios::binary);
	if(fout.is_open()==false)
	{
		cerr<<"打开文件失败";
		exit(0);
	}

	bool flag =false; 
	int fd ;
	Packet p ;
    Packet tmp_buffer;
    bool tag = true;

	while(true)
	{

/* 		while(nDl_to_net == 0)
			sleep(1);
		nDl_to_net --; */
		network_from_datalink(p);
		

		if(allZero(p)) //如果全0包
		{
            int i = MAX_PKT - 1;
            for(; i >= 0; i --)
                if(tmp_buffer.data[i] != '\0')
                    break;

            for(int j=0 ; j<=i ; j++)
                fout<<tmp_buffer.data[j];
			fout.close();
			cout <<"recv done !\n";
			kill(getppid(),60);
			exit(0);
		}

        if(tag == false)  //  不是第一次
            for (int i = 0 ; i<sizeof(Packet);i++)
                fout<<tmp_buffer.data[i];

        memcpy(&tmp_buffer, &p, sizeof(Packet));
        tag = false;

		cout <<"net recv ok ！!\n";

	}


}

void receive2_dtlink()
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

	int i =0;
	Frame s_ack ;
	s_ack.kind = htonl(ack);
	s_ack.seq = 0xffffffff;
	while(true)
	{
		datalink_from_physical(s);
		datalink_to_network_layer(s.info);

		s_ack.ack = i++;
		if(randProb(0.1))
		{
			//sleep(1);
			mySleep(0,50);
			myLog("ack delay !",s_ack);
		}
		datalink_to_physical(s_ack);
		kill(ps_pid,SIG_DATALK_TO_PHYSIC);
		cout <<" dl to net ok !\n";

	}


}

void receive2_physic(int argc , char *argv[])
{

	sleep(1);
	struct sockaddr_in server_addr = {0};
	int port =atoi(argv[1]);
	server_addr.sin_port = htons(port);
	server_addr.sin_family=AF_INET;


 	int server_fd ;
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		myExit();
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		myExit();
	if (listen(server_fd, 5)==-1)
		myExit(); 

	int cfd = accept(server_fd, NULL,NULL);

	if(cfd <0)
	{
		cerr<<"accept error !\n";
		cout<<nt_pid<<' '<<dl_pid<<" "<<ps_pid<<endl;   // bug
		exit(0);
	}

	cout<<"cfd = "<<cfd<<endl;
	setNonBlock(cfd);
	ps_pid = getpid();

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
	setTime(timeout,0,50);

	int dataLen ;

	while(true)
	{
		rset_cpy = fdset;
		setTime(timeout,1,10);
		if(nDl_to_phy)
		{ 
			cout <<"catch kill ！\n"; 
			wset_cpy = fdset; 
		} 
		else 
			FD_ZERO(&wset_cpy); 

		switch(select(cfd+1,&rset_cpy,&wset_cpy,NULL,&timeout))
		{
			case -1 :
				break;
			case 0:
				cout <<"time out "<<endl;
				break;
				//cout<<"time out "<<endl;
			default:
			if(FD_ISSET(cfd,&rset_cpy))
			{
				// 先不考虑读不满的情况 
				cout <<"start read !\n";
				dataLen = sizeof(s_recv);
				int recvNum = recv(cfd, &s_recv, dataLen, 0);
				if (recvNum <= 0)
				{ 
					cout << "recv err" << endl;
					exit(0);
				} 
				else
				{
					cout<<"read ok "<<recvNum<<endl;
					physical_to_datalink(s_recv);
					//kill(dl_pid , SIG_FRAME_ARRIVAL);
				}
			}
			if(FD_ISSET(cfd,&wset_cpy))
			{
				cout <<"start write !\n";
				physical_layer_from_datalink(s);
				//dataLen = frameSize(s);
				dataLen = sizeof(s);
				int sndNum = send(cfd, &s, dataLen , 0);
				if (sndNum > 0){
					cout<<"snd ok ！"<<sndNum<<endl;
					nDl_to_phy--;
				}
				else
				{
					cerr << "send failed !\n";
					break;
				}

			}
		}



		


	}

	cout<<"receive2_physic ok !\n";
	exit(0);
}

void fork_layer(int argc , char *argv [])
{

	pid_t pid ;
	Init();


	ofstream fout (pidFileName,ios::out);

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
				receive2_network();
			else if (i==1)
				receive2_dtlink();
			else if (i==2)
				receive2_physic(argc , argv );
			cout <<i<<" "<<"23333"<<endl;
		}
	}
	cout <<" fork layers ok !\n";
	return ;
}

int main(int argc , char *argv[])
{
	srand(time(0));
	signal(SIGCHLD,SIG_IGN);
	create_daemon();
	sig_catch();
	fork_layer(argc, argv);

	while(1)
		sleep(1);

}


