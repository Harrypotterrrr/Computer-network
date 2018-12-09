#include "../common/common.h"


#define stransDataName "send_data"
#define sbuffername "send_network_buffer"

const char *pidFileName = "filepid";

#define rtransDataName "recerive_data"
#define rbuffername "recv_network_buffer"



// 设置缓冲区域大小


const int buffSize = 1 ;
const int window_size=8;
const int wait_time=500;
void network()
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
	fout.open(rtransDataName,ios::out|ios::binary);
	FILE* fin=fopen(stransDataName,"rb");
	if(fout.is_open()==false || fin==NULL)
	{
		cerr<<"打开文件失败";
		kill(getppid() , 60);
		exit(0);
	}
	bool flag =false; 
	int fd ;
	Packet p ;
	Packet prep;
	int flag_prep=0;
	event_type myevent;
	int sended=0,recved=0;
	int read_cnt;
	while(true)
	{
		myevent=noevent;
		if(nNet_to_dl < buffSize && !sended){ //写
			nNet_to_dl ++;
			memset(p.data, 0, sizeof(p));
			read_cnt=fread((char *)&p,1,sizeof(p),fin);
			if(read_cnt==0){
				sended=1;
				cout<<"network send done \n";
				fclose(fin);
			}
			network_to_datalink(p,1);
			kill(dl_pid,SIG_NETWORK_TO_DATALK);

			cout <<"send1 net to dt link ok !\n";

		}
		else{ //读
			if(nDl_to_net){
				nDl_to_net--;
				network_from_datalink(p);
				if(allZero(p)){
					int len=sizeof(Packet)-1;
					while(len>=0 && prep.data[len]==0)
						len--;
					for (int i = 0 ; i<=len;i++)
						fout<<prep.data[i];
					recved=1;
					fout.close();
					cout <<"recv done !\n";
				}
				else{
					if(flag_prep){
						for (int i = 0 ; i<sizeof(Packet);i++)
							fout<<prep.data[i];
					}
					prep=p;
					flag_prep=1;
				}
				cout <<"net recv ok ！!\n";
				kill(dl_pid,SIG_NETWORK_FROM_DATALK);
			}else{
				sleep(1);
			}
		}

		if(sended && recved){
			kill(dl_pid,SIG_DONE);
			kill(getppid() , 60);
			exit(0);
		}
	}

}
void dtlink()
{
	Frame r,s ;
	Packet buffer ;
	while(PL_Connect==false)  
		sleep(1);
	timer_t mytimer[2];
	cout <<"dtlink conn ok !\n";
	getLayerPid(nt_pid,dl_pid,ps_pid,pidFileName);
	cout <<"dtlink getpid ok !\n";
	kill(nt_pid ,SIG_PL_CONNECT);

	connectFifo();

	int count =0;
	seq_nr next_frame_to_send=0,frame_expected=0;
	event_type myevent;	

	while(!nNet_to_dl)
		sleep(1);
	datalink_from_network(s.info,1);
	nNet_to_dl--;
	kill(nt_pid,SIG_DATALK_FROM_NETWORK);
	s.kind = data;
	s.seq = next_frame_to_send;
	s.ack = 1^frame_expected; 
	datalink_to_physical(s);
	kill(ps_pid,SIG_DATALK_TO_PHYSIC);
	cout <<"write to pysical ok !\n";
	//mytimer[0]=start_timer(wait_time);
	while(true)
	{
		myevent=noevent;
		wait_for_event(myevent);
		if(myevent==done){
			kill(ps_pid,SIG_DONE);
			kill(getppid() , 60);
			exit(0);
		}else if(myevent==timeout){
			//stop_timer(mytimer[0]);
			//mytimer[0]=start_timer(wait_time);
		}
		else if(myevent==frame_arrival){
			datalink_from_physical(r);
			cout <<"dt link read ok !\n";
			if(r.seq==frame_expected){
				frame_expected^=1;
				datalink_to_network_layer(r.info);
				kill(nt_pid,SIG_DATALK_TO_NETWORK);
			}
			if(r.ack==next_frame_to_send){
				stop_timer(mytimer[0]);
				if(nNet_to_dl==0)
					continue;
				nNet_to_dl-- ;
				datalink_from_network(s.info,1);
				next_frame_to_send^=1;
				kill(nt_pid,SIG_DATALK_FROM_NETWORK);
				//mytimer[0]=start_timer(wait_time);
			}
		}
		
		s.kind = data;
		s.seq = next_frame_to_send;  //窗口大小只有一个
		s.ack = 1^frame_expected;  //不需
		datalink_to_physical(s);
		kill(ps_pid,SIG_DATALK_TO_PHYSIC);
		cout <<"write to pysical ok !\n";
		
	}
	exit(0);
}
void sender4_physic(int argc , char *argv[])
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
		kill(getppid() , 60);
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
		
		if(!eventQueue.empty()){
			event_type myevent=noevent;
			wait_for_event(myevent);
			if(myevent==done){
				kill(getppid() , 60);
				exit(0);
			}
		}
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
					cout << "done" << endl;
					kill(getppid(),60);
					exit(0);
				}
				else
				{
					cout <<"recv ok ! "<<recvNum<<endl;
					physical_to_datalink(s_recv);
					s_recv.ack=ntohl(s_recv.ack);
					s_recv.seq=ntohl(s_recv.seq);
					s_recv.kind=ntohl(s_recv.kind);
					kill(dl_pid , SIG_FRAME_ARRIVAL);
				}
			}
			if(FD_ISSET(cfd,&wset_cpy))
			{
				cout <<"start write !\n";
				if(physical_from_datalink(s)==FALSE)
				{
					cerr<<"from physical failed !\n";
					exit(0);
					break;
				}

				dataLen = sizeof (s);
				//dataLen = frameSize(s);
				cout <<"dataLen "<< frameSize(s)<<endl;
				s.ack=htonl(s.ack);
				s.seq=htonl(s.seq);
				s.kind=htonl(s.kind);
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
				network();
			else if (i==1)
				dtlink();
			else if (i==2)
				sender4_physic(argc , argv );
			cout <<i<<" "<<"23333"<<endl;
		}
	}
	cout <<" fork layers ok !\n";
	return ;
}

int exit_child=0;
void SIG_CNT_CHILD(int num){
		exit_child++;
}
int main(int argc , char *argv[])
{
	signal(SIGCHLD,SIG_CNT_CHILD);
	signal(60,SIG_CNT_CHILD);
	create_daemon();
	sig_catch();
	fork_layer(argc, argv);

	while(exit_child<3)
		sleep(1);

}
