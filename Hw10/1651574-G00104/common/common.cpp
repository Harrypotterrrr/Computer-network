#include "common.h"

//当前网络层向链路层的滑窗中 帧的个数
int nNet_to_dl ;

int nDl_to_phy;
int nPhy_to_dl;
int nDl_to_net;

bool PL_Connect ;
int fifo_dtop;
int fifo_ptod;
int fifo_dton;

pid_t nt_pid; // 网络层
pid_t dl_pid; // 数据链路层
pid_t ps_pid; // 物理层

queue<event_type> eventQueue ; 

void getTestPath(int k, char *path)
{
	char tmp[10];
	memset(path,0,PATHLENGTH);
	memset(tmp,0,10);
	strcpy(path,FIFO_NT_TO_DT);
	sprintf(tmp,"%d",k);
	strcat(path,tmp);
}


void Init()
{
	srand(time(0));
	eventQueue = queue<event_type>();
	unlink (FIFO_TO_NETWORK);
	unlink (FIFO_TO_PHYSICAL);
	unlink (FIFO_PH_TO_DT);
	if(mkfifo(FIFO_TO_NETWORK,S_IFIFO|0666)==-1 ) {
		cerr << "create" << FIFO_TO_NETWORK << "fail!" << endl;
		myExit();
	}
	if(mkfifo(FIFO_TO_PHYSICAL,S_IFIFO|0666)==-1 ) {
		cerr << "create" << FIFO_TO_PHYSICAL << "fail!" << endl;
		myExit();
	}		
	if(mkfifo(FIFO_PH_TO_DT,S_IFIFO|0666)==-1 ) {
		cerr << "create" << FIFO_PH_TO_DT << "fail!" << endl;
		myExit();
	}	
	cout <<"init ok !\n";
	return ;

}

Status network_to_datalink(Packet& buffer, int k)
{
    char path[PATHLENGTH];
    getTestPath(k, path);
	cout <<path<<endl;
    int fd=open(path, O_CREAT | O_WRONLY,0666);
    if(fd<0) {
        cerr << "open" << path << "fail!" <<endl;
		return FALSE;
    }
    // 写锁
    if(lock_set(fd,F_WRLCK)==FALSE) 
        myExit();
    
    
    if(write(fd,&buffer,BUFSIZE)<0) {
        cerr << "write" << path << "fail!" <<endl;
        lock_set(fd,F_UNLCK); 
        myExit();
    }
    lock_set(fd,F_UNLCK);
    close(fd);
    return OK;
}

Status datalink_from_network(Packet& buffer, int k)
{
    char path[PATHLENGTH];
    getTestPath(k, path);

	int fd;
	fd=open(path,O_RDONLY,0666);//打开共享文件
	if(fd<0) {
        cerr << "open" <<path << "fail!" <<endl;
		return FALSE;
	}
	
	// 给文件上读入锁 
	lock_set(fd, F_RDLCK); 

    if (read(fd,buffer.data, BUFSIZE) < 0) {
        cerr << "read" <<path << "fail!" <<endl;
		lock_set(fd, F_UNLCK); 
		return FALSE;
	}   

	lock_set(fd, F_UNLCK);    
	close(fd);  
    //unlink(path);
	return OK;
}

Status datalink_from_physical(Frame &r)
{

	//int fd=open(FIFO_PH_TO_DT,O_RDONLY);
	int fd = fifo_ptod;
	if(fd<0) {
		cerr << "open" << FIFO_PH_TO_DT << "fail!" << endl;
		return FALSE ;
	}
	if((read(fd,&r,FRAMESIZE))<=0) {
        cerr << "Read" << FIFO_PH_TO_DT << "fail!" <<endl;
		return FALSE;
	}
/* 	if((read(fd,&r,12))<=0) {
        cerr << "Read" << FIFO_PH_TO_DT << "fail!" <<endl;
		return FALSE;
	}
	else if(frameSize(r)>12)
		read(fd,&r+12,FRAMESIZE-12);
 */

	return OK;
}


Status datalink_to_network_layer(Packet &buffer)
{

	//int fd=open(FIFO_TO_NETWORK,O_WRONLY);
	int fd = fifo_dton;
	if(fd<0) {
		cerr << "open1" << FIFO_TO_NETWORK << "fail!" <<fd << endl;
	}
	if((write(fd,&buffer,BUFSIZE))<0) {
        cerr << "Write" << FIFO_TO_NETWORK << "fail!" <<endl;
		return FALSE;
	}

	return OK;
}

Status datalink_to_physical(Frame &s)
{
	//int fd=open(FIFO_TO_PHYSICAL,O_WRONLY);
	int fd=fifo_dtop;

	if(fd<0) {
		cerr << "open" << FIFO_TO_PHYSICAL << "fail!" << endl;
		return FALSE ;
	}
	if((write(fd,&s,FRAMESIZE))<0) {
        cerr << "write" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		myExit();
	}

/* 	if((write(fd,&s,frameSize(s)))<0) {
        cerr << "write" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		myExit();
	} */


	return OK;
}



Status network_from_datalink (Packet & buffer )
{
	//int fd=open(FIFO_TO_NETWORK,O_RDONLY);
	int fd = fifo_dton;
	if(fd<0) {
		cerr << "open2" << FIFO_TO_NETWORK << "fail!" << endl;
		return FALSE;
	}
	if((read(fd,&buffer,BUFSIZE))<0)
	{
        cerr << "Write" << FIFO_TO_NETWORK << "fail!" <<endl;
		return FALSE;
	}

	return OK;
}

Status physical_to_datalink(Frame & r )
{

	//int fd=open(FIFO_PH_TO_DT,O_WRONLY);
	int fd = fifo_ptod;
	//cout <<1233333<<endl;
	if(fd<0) {
		cerr << "open" << FIFO_PH_TO_DT << "fail!" << endl;
		return FALSE;
	}
	if((write(fd,& r ,FRAMESIZE))<0) {
        cerr << "Write" << FIFO_PH_TO_DT << "fail!" <<endl;
		return FALSE;
	}
/* 	if((write(fd,& r ,frameSize(r)))<0) {
        cerr << "Write" << FIFO_PH_TO_DT << "fail!" <<endl;
		return FALSE;
	} */

	return OK;
}

Status physical_from_datalink(Frame & r)
{
	int fd = fifo_dtop;
	if(fd<0) {
		cerr << "open " << FIFO_TO_PHYSICAL << "fail!" << endl;
		return FALSE;
	}
	if((read(fd,&r,FRAMESIZE))<0) {
        cerr << "Write" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
/* 	if((read(fd,&r,12))<0) {
        cerr << "Write" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
	else if (frameSize(r)>12)
	{
		read(fd,&r+12,FRAMESIZE-12);
	} */
	return OK;
}


// 信号处理函数
void sig_func(int sig)  
{  
	cout <<"sig ="<<sig<<endl;
	switch(sig)
	{

		case SIGCHLD:
			exit(0);
			break ;
		//  控制网络层到链路层的数据
		case SIG_NETWORK_TO_DATALK:
			nNet_to_dl++;
			break;
		case SIG_DATALK_FROM_NETWORK:
			nNet_to_dl --;
			break;
		case SIG_DATALK_TO_NETWORK:
			nDl_to_net++;
			break;
		case SIG_NETWORK_FROM_DATALK:
			nDl_to_net--;
			break;
		
		case SIG_PL_CONNECT:
			cout <<"conn ok!\n";
			PL_Connect=true;
			break;

		case SIG_CHSUM_ERR:
			eventQueue.push(chsum_err);
			break ;
		
		case SIG_FRAME_ARRIVAL :
			eventQueue.push(frame_arrival);
			break ;
		
		case SIGALRM:
			cout <<"-----------time out ------------"<<endl;
			eventQueue.push(timeout);
			break ;

		case SIG_ENABLE_NETWORK_LAYER:

			break;
		
		case SIG_DISABEL_NETWORK_LAYER:

			break ;

		case SIG_DATALK_TO_PHYSIC:
			nDl_to_phy ++ ;
			break ;
		case SIG_PHYSIC_FROM_DATALK:
			nDl_to_phy -- ;
			break ;
		case SIG_PHYSIC_TO_DATALK:
			nPhy_to_dl ++ ;
			break ;
		case SIG_DATALK_FROM_PHYSIC:
			nPhy_to_dl -- ;
			break ;

		case SIG_DONE:
			eventQueue.push(done);
			break ;
		default :
			break ;
	
	}
}  

void sig_catch()
{
	signal(SIGCHLD,sig_func);
	signal(SIG_NETWORK_FROM_DATALK,sig_func);
	signal(SIG_NETWORK_TO_DATALK,sig_func);
	signal(SIG_DATALK_FROM_NETWORK,sig_func);
	signal(SIG_DATALK_TO_NETWORK,sig_func);
	signal(SIG_DATALK_FROM_PHYSIC,sig_func);
	signal(SIG_PHYSIC_FROM_DATALK,sig_func);
	signal(SIG_PHYSIC_TO_DATALK,sig_func);
	signal(SIG_DATALK_TO_PHYSIC,sig_func);

	signal(SIG_DONE,sig_func);
	signal(SIG_PL_CONNECT,sig_func);

	signal(SIG_CHSUM_ERR,sig_func); 
	signal(SIG_FRAME_ARRIVAL,sig_func);
	signal(SIG_NETWORK_LAYER_READY,sig_func);
	signal(SIG_ENABLE_NETWORK_LAYER,sig_func);
	signal(SIG_DISABEL_NETWORK_LAYER,sig_func);
	signal(SIGALRM,sig_func);
}


void enable_network_layer()
{
	//获取network层进程的pid号
    int pid = nt_pid ;
	kill(pid,SIG_ENABLE_NETWORK_LAYER);
}

void disable_network_layer()
{
	//获取network层进程的pid号
    int pid = nt_pid ;
	kill(pid,SIG_DISABEL_NETWORK_LAYER);
}

Status lock_set(int fd, int type)  
{  
    struct flock old_lock, lock;  
    lock.l_whence = SEEK_SET;  
    lock.l_start = 0;  
    lock.l_len = 0;  
    lock.l_type = type;  
    lock.l_pid = -1;  
      
    // 判断文件是否可以上锁   
    fcntl(fd, F_GETLK, &lock);  
      
    if (lock.l_type != F_UNLCK)  
    {  
        //判断文件不能上锁的原因 
        if (lock.l_type == F_RDLCK) 
           ;// cout << "Read lock already set by" << lock.l_pid <<endl;
        else if (lock.l_type == F_WRLCK)   
           ;// cout << "Write lock already set by" << lock.l_pid <<endl;
    }  
      
    lock.l_type = type; 
  //  cout << "The type of lock is" << type <<endl;
    if ((fcntl(fd, F_SETLKW, &lock)) < 0) {  
       // cout << "Lock failed " << lock.l_type <<endl;
        return FALSE;  
    }  
          
    switch(lock.l_type)  
    {  
        case F_RDLCK:  
            //cout << "Read lock set by" << lock.l_pid <<endl;
            break;  
  
        case F_WRLCK:  
           // cout << "Write lock set by" << lock.l_pid <<endl;
            break;  
  
        case F_UNLCK:  
           // cout << "Release lock set by" << lock.l_pid <<endl;
            break; 
        default:  
            break;  
    }
    return OK;  
}  

void generateData(char buf[])
{
	// 若没填满1024字节，填充尾零
	memset(buf,0,BUFSIZE);
	for(int i=0;i<BUFSIZE;i++)
		buf[i]=rand()%10+'0';
}

void create_daemon()
{
	pid_t pid = fork();
	if(pid==-1)
		exit(-1);
	if(pid>0)
		exit(0);

	if(setsid()==-1)
		exit(-1);
	chdir("./");
	umask(0);
	pid = fork();
	if(pid ==-1)
		exit(-1);
	if(pid>0)
		exit(0);

	signal(SIGCHLD, SIG_IGN); 
	
	return ;
}

void myExit()
{
	cerr << strerror(errno) << endl;
	exit(1);
}

int getLayerPid(int & net , int & dt , int & ps ,const char * filename)
{
	ifstream fin (filename,ios::in);
	if(fin.is_open()==false)
	{
		cerr<<"getLayerPid failed !\n";
		return -1 ;
	}
	
	//fin<<net<<dt<<ps;
	fin>>net>>dt>>ps;
	fin.close();
	cout <<"getLayer pid ok ";
	return 0;
}

 bool allZero(const Packet & p)
 {
	 int i ;
	 for(int i = 0 ;i <sizeof(p);i++)
	 	if(p.data[i]!=0)
		 	return false;
	return true;

 }

 void connectFifo()
 {
	 int pid = getpid();
	 if(pid == nt_pid)
	 {
		fifo_dton = open(FIFO_TO_NETWORK,O_RDONLY);
	 }
	 else if (pid==dl_pid)
	 {
		fifo_dton = open(FIFO_TO_NETWORK,O_WRONLY);
		fifo_dtop = open(FIFO_TO_PHYSICAL,O_WRONLY);
		fifo_ptod = open(FIFO_PH_TO_DT,O_RDONLY);
	 }
	 else if (pid==ps_pid)
	 {
		fifo_dtop = open(FIFO_TO_PHYSICAL,O_RDONLY);
		fifo_ptod = open(FIFO_PH_TO_DT,O_WRONLY);
	 }
 }

 void wait_for_event( event_type &myevent)
 {
	 while(eventQueue.empty())
	 	sleep(1);
	
	myevent = eventQueue.front();
	eventQueue.pop();
	return ;
 }

 int frameSize( const Frame & s )
 {
	 if(s.kind==htonl(data))
	 	return 1036;
	else if(s.kind==htonl(ack)||s.kind==htonl(nak))
		return 12;
	else 
	{
		cerr <<"failed !\n";
		exit(0);
	}
 }

 void setNonBlock(int fd)
{
	int flag;
	if ((flag = fcntl(fd, F_GETFL, 0)) == -1)
		myExit();
	if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
		myExit();
}

void setTime(timeval &time_out, int sec, int usec )
{
	time_out.tv_sec = sec;
	time_out.tv_usec = usec;
	return;
}

void myLog(const char *msg , const Frame & r )
{

	ofstream fout ("log",ios::out|ios::app);
	fout <<msg<< " seq ="<<hex<<r.seq<<" ack = "<<dec<<r.ack<<endl;
	fout.close();
	return ;
}

// 为真的概率
bool randProb(double x )
{
	double p = rand()/ (RAND_MAX+1e-6);
	if(p<x)
		return true;
	return false ;
}

// 睡眠时间
void mySleep( int s ,int us )
{
	timeval time_out = {s,us};
	select(10,NULL,NULL,NULL,&time_out);
	return ;
}

timer_t start_timer()
{
    struct sigevent evp;
    struct itimerspec ts;
    timer_t timer;

	cout <<"setOk"<<endl;

	evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGALRM;


	int ret = timer_create(CLOCK_REALTIME, &evp, &timer);



	return timer ;

}

void set_timer (timer_t timer )
{
	struct itimerspec ts;
	ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 1000*1000*50;

	timer_settime(timer, 0 ,&ts , NULL);
}



void stop_timer (timer_t timer )
{
	timer_delete (timer);

}