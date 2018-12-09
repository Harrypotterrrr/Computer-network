#include "../common/common.h"


#define transDataName "send1_data"
#define buffername "send1_network_buffer"

const char *pidFileName = "send1pid";



// 设置缓冲区域大小

const int buffSize = 1 ;


void send1_network()
{
	
	while(PL_Connect==false)
		sleep(1);
	cout <<"network conn ok !\n";

	getLayerPid(nt_pid,dl_pid,ps_pid,pidFileName);
	cout <<"network read pid ok "<<endl;
	unlink(pidFileName);

	//fifo_dton = open(FIFO_TO_NETWORK,O_RDONLY);

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
		//  如果缓存慢 则入睡
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
			kill(dl_pid,SIG_DONE);
			cout<<"send1 network layer done \n";
			fin.close();
			return ;
		}

		
	}
}

void send1_dtlink()
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

	int count =0;
	while(true)
	{
		while(nNet_to_dl==0)
			sleep(1);
		cout <<"dt link read ok !\n";
		nNet_to_dl-- ;
		datalink_from_network(s.info,1);
		s.kind = data;
		s.seq = 1 ;  //窗口大小只有一个
		//s.ack = ;  //不需要

		kill(nt_pid,SIG_DATALK_FROM_NETWORK);

		cout <<"ready to write pysical !\n";
		datalink_to_physical(s);
		cout <<"write to pysical ok !\n";

	}
	exit(0);

}

void send1_physic(int argc , char *argv[])
{

	//sleep(1);

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


	cout<<"cfd = "<<cfd<<endl;

	ps_pid = getpid();

	PL_Connect = true ;
	kill(dl_pid ,SIG_PL_CONNECT);

	connectFifo();

	Frame s ;
	Frame s_recv ; 

	while(true)
	{
		physical_from_datalink(s);

		int sndNum = send(cfd,&s , sizeof(s),0);
		if(sndNum>0)
		{
			cout<<"send ok ! "<<sndNum<<endl;

			cout <<endl;
		}
		else 
			cerr<<"send failed !\n";

/* 		int recvNum = recv(cfd,&s_recv,sizeof(s_recv),0);
		if(recvNum <0)
		{
			cout <<"recv err"<<endl;
			exit(0);
		}
		else 
		{
			if(s_recv.kind==ack)
				kill(dl_pid,SIG_FRAME_ARRIVAL);
			else if (s_recv.kind==data)
				physical_to_datalink(s_recv);
			else if (s_recv.kind==nak)
				kill(dl_pid,SIG_CHSUM_ERR);
		} */

		


	}
	cout<<"send1_physic ok !\n";
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
				send1_network();
			else if (i==1)
				send1_dtlink();
			else if (i==2)
				send1_physic(argc , argv );
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
