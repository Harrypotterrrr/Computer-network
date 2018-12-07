#include "common.h"

Status from_network_layer(Packet* buffer,char sharedFilePath[])
{
	int fd;
	fd=open(sharedFilePath,O_RDONLY,0666);//打开共享文件
	if(fd<0) {
        cerr << "open" <<sharedFilePath << "fail!" <<endl;
		return FALSE;
	}
	
	// 给文件上读入锁 
	lock_set(fd, F_RDLCK); 

    if (read(fd,buffer->data, BUFSIZE) < 0) {
        cerr << "read" <<sharedFilePath << "fail!" <<endl;
		lock_set(fd, F_UNLCK); 
		return FALSE;
	}   

	lock_set(fd, F_UNLCK);    
	close(fd);  
	return OK;
}

Status to_physical_layer(Frame* s)
{
    // 有名管道
	if(mkfifo(FIFO_TO_PHYSICAL,0666)==-1 && errno!=EEXIST) {
        cerr << "create" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
	
	int fd=open(FIFO_TO_PHYSICAL,O_WRONLY,0666);
	if(fd<0) {
        cerr << "open" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
	if((write(fd,s,FRAMESIZE))<0) {
        cerr << "write" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
	close(fd);
	return OK;
}

Status from_physical_layer(Frame* r)
{
	if(mkfifo(FIFO_TO_DATALINK,0666)==-1 && errno!=EEXIST) {
        cerr << "create" << FIFO_TO_DATALINK << "fail!" <<endl;
		return FALSE;
	}
	
	int fd=open(FIFO_TO_DATALINK,O_RDONLY,0666);
	if(fd<0) {
        cerr << "open" << FIFO_TO_DATALINK << "fail!" <<endl;
		return FALSE;
	}
	if((read(fd,r,FRAMESIZE))<0) {
        cerr << "Read" << FIFO_TO_DATALINK << "fail!" <<endl;
		return FALSE;
	}
	close(fd);
	return OK;
}

Status to_network_layer(Packet* buffer)
{
	if(mkfifo(FIFO_TO_NETWORK,0666)==-1 && errno!=EEXIST) {
        cerr << "create" << FIFO_TO_NETWORK << "fail!" <<endl;
		return FALSE;
	}
	
	int fd=open(FIFO_TO_NETWORK,O_WRONLY,0666);
	if(fd<0) {
        cerr << "open" << FIFO_TO_NETWORK << "fail!" <<endl;
		return FALSE;
	}
	if((write(fd,buffer,BUFSIZE))<0) {
        cerr << "Write" << FIFO_TO_NETWORK << "fail!" <<endl;
		return FALSE;
	}
	close(fd);
	return OK;
}

Status physical_layer_from_datalink(Frame *s)
{
	if(mkfifo(FIFO_TO_PHYSICAL,0666)==-1 && errno!=EEXIST) {
        cerr << "create" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
	
	int fd=open(FIFO_TO_PHYSICAL,O_RDONLY,0666);
	if(fd<0) {
        cerr << "Open" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
	if((read(fd,s,FRAMESIZE))<0)
	{
        cerr << "Write" << FIFO_TO_PHYSICAL << "fail!" <<endl;
		return FALSE;
	}
	close(fd);
	unlink(FIFO_TO_PHYSICAL);
	return OK;
}


void sig_func(int sig)  
{  
	int sig_num=sig;
}  

void sig_catch()
{
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
    int pid;
    FILE *fp = popen("ps -e | grep \'_network' | awk \'{print $1}\'","r");
    char buffer[10] = {0};
    fgets(buffer, 10, fp);
    pid=atoi(buffer);
	kill(pid,SIG_ENABLE_NETWORK_LAYER);
}

void disable_network_layer()
{
	//获取network层进程的pid号
    int pid;
    FILE *fp = popen("ps -e | grep \'_network' | awk \'{print $1}\'","r");
    char buffer[10] = {0};
    fgets(buffer, 10, fp);
    pid=atoi(buffer);
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
            cout << "Read lock already set by" << lock.l_pid <<endl;
        else if (lock.l_type == F_WRLCK)   
            cout << "Write lock already set by" << lock.l_pid <<endl;
    }  
      
    lock.l_type = type; 
    cout << "The type of lock is" << type <<endl;
    if ((fcntl(fd, F_SETLKW, &lock)) < 0) {  
        cout << "Lock failed " << lock.l_type <<endl;
        return FALSE;  
    }  
          
    switch(lock.l_type)  
    {  
        case F_RDLCK:  
            cout << "Read lock set by" << lock.l_pid <<endl;
            break;  
  
        case F_WRLCK:  
            cout << "Write lock set by" << lock.l_pid <<endl;
            break;  
  
        case F_UNLCK:  
            cout << "Release lock set by" << lock.l_pid <<endl;
            break; 
        default:  
            break;  
    }
    return OK;  
}  

void getSharedFilePath(int k,char path[])
{
	char tmp[10];
	memset(path,0,PATHLENGTH);
	memset(tmp,0,10);
	strcpy(path,"network_datalink.share.");
	sprintf(tmp,"%04d",k);
	strcat(path,tmp);
}

void getTestPath(int k,char path[])
{
	char tmp[10];
	memset(path,0,PATHLENGTH);
	memset(tmp,0,10);
	strcpy(path,"1651574.");
	sprintf(tmp,"%04d",k);
	strcat(path,tmp);
}

void generateData(char buf[])
{
	// 若没填满1024字节，填充尾零
	memset(buf,0,BUFSIZE);
	for(int i=0;i<BUFSIZE;i++)
		buf[i]=rand()%10+'0';
}
