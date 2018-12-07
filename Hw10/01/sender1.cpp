#include "../common/common.h"

int main()
{
	pid_t nt_pid; // �����
	pid_t dl_pid; // ������·��
	pid_t ps_pid; // �����
	
	while((nt_pid=fork())<0)
		sleep(1);
	
	if(nt_pid==0) { // �����
		char new_name[20] = "send1_network"; 
		prctl(PR_SET_NAME, new_name);
		
		int k=1;
		char sharedFilePath[PATHLENGTH];
		int fd;
		char buf[BUFSIZE];
		while(true) {
			getSharedFilePath(k,sharedFilePath);
			fd=open(sharedFilePath, O_CREAT | O_WRONLY,0666);
			if(fd<0) {
                cerr << "open" << sharedFilePath << "fail!" <<endl;
				continue;
			}
            // д��
			if(lock_set(fd,F_WRLCK)==FALSE) 
				exit(-1); 
			
			generateData(buf); // debug
			
			if(write(fd,buf,BUFSIZE)<0) {
                cerr << "write" << sharedFilePath << "fail!" <<endl;
				lock_set(fd,F_UNLCK); 
				exit(-1);
			}
			lock_set(fd,F_UNLCK); 
			close(fd);
			k++;
			sleep(1);
			if(k>10)   // debug
				break;
		}
		while(true)
			sleep(1);
		exit(0);
	}
	else if(nt_pid>0) {
		while((dl_pid=fork())<0)
			sleep(1);
		if(dl_pid==0) { // ������·��
		
			char new_name[20] = "send1_dtlink";
			prctl(PR_SET_NAME, new_name);
			
			Frame s;
			Packet buffer;
			int k=1;
			char sharedFilePath[PATHLENGTH];
			
			while(true) {
				sleep(1); //�ȴ��������д����!
				getSharedFilePath(k,sharedFilePath);
				from_network_layer(&buffer,sharedFilePath);
				for(int i=0;i<BUFSIZE;i++)
					(s.info.data)[i]=(buffer.data)[i];
				to_physical_layer(&s);
				k++;
				if(k>10)   // debug
					break;
			}
			while(true)
				sleep(1);
			exit(0);			
		}
		else if(dl_pid>0)
		{
			while((ps_pid=fork())<0)
				sleep(1);
			if(ps_pid==0) // �����
			{
				char new_name[20] = "send1_physic";
				prctl(PR_SET_NAME, new_name);
				Frame s;
				Packet buffer;
				
				int k=1;
				int fd;
				char testPath[PATHLENGTH];
				while(true)
				{
					physical_layer_from_datalink(&s);
					getTestPath(k,testPath);
					fd=open(testPath,O_CREAT | O_WRONLY ,0666);
					if(fd<0) {
                        cerr << "open" << testPath << "fail!" <<endl;
						continue;
					}
					if(write(fd,&s,FRAMESIZE)<0) {
                        cerr << "write" << testPath << "fail!" <<endl;
						exit(-1);
					}
					k++;
					if(k> 10) // debug
						break;
				}
				while(true)
					sleep(1);
				exit(0);
			}
			else
				exit(0);
		}
	}
}