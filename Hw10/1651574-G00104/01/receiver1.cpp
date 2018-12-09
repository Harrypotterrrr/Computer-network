#include "../common/common.h"

#define transDataName "recerive_data"
#define buffername "send1_network_buffer"

const char *pidFileName = "receive1pid";

// 链路层到网络层的个数
//int ndl_to_net = 0 ;
//int nphy_todl = 0;


void receive1_network()
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
	fout.open(transDataName,ios::out|ios::binary|ios::app);
	if(fout.is_open()==false)
	{
		cerr<<"打开文件失败";
		exit(0);
	}

	bool flag =false; 
	int fd ;
	Packet p ;


	while(true)
	{

/* 		while(nDl_to_net == 0)
			sleep(1);
		nDl_to_net --; */
		network_from_datalink(p);
		
		if(allZero(p)) //如果全0包
		{
			// TODO  删除前导
			fout.close();
			cout <<"recv done !\n";
			kill(getppid(),60);
			exit(0);
		}
		else
			for (int i = 0 ; i<sizeof(Packet);i++)
				fout<<p.data[i];

		cout <<"net recv ok ！!\n";

	}


}

void receive1_dtlink()
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


	while(true)
	{

		datalink_from_physical(s);


		datalink_to_network_layer(s.info);
		cout <<" dl to net ok !\n";

	}


}

void receive1_physic(int argc , char *argv[])
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
		cout<<nt_pid<<' '<<dl_pid<<" "<<ps_pid<<endl;
		exit(0);
	}

	cout<<"cfd = "<<cfd<<endl;

	ps_pid = getpid();

	kill(dl_pid ,SIG_PL_CONNECT);
	connectFifo();

	cout <<"send ok !\n";

	Frame s ;

	while(true)
	{
		int recvNum = recv(cfd,&s,sizeof(Frame),0);
		if(recvNum<=0)
		{
			cout <<"recv err"<<endl;
			exit(0);
		}
		else 
			cout<<"recv ok "<<recvNum<<endl;

		cout <<"npy to dl "<<nPhy_to_dl<<endl;
		physical_to_datalink(s);

		cout <<"send signl ok "<<endl;
	}

	cout<<"receive1_physic ok !\n";
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
				receive1_network();
			else if (i==1)
				receive1_dtlink();
			else if (i==2)
				receive1_physic(argc , argv );
			cout <<i<<" "<<"23333"<<endl;
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


