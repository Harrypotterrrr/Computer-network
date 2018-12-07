#include "myutil.h"

struct sockaddr_in server_addr;
int port;
bool flag_block;
bool flag_fork;
int max_num;

const char *orderMsg[]={
	"StuNo",
	"pid",
	"TIME",
	"str",
	"end"
};

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
	exit(EXIT_FAILURE);
}

void setReusePort(int fd )
{
	int enable = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
		myExit();
}

void setTime(timeval &timeout, int sec, int usec )
{
	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	return;
}

void setNonBlock(int fd)
{
	int flag;
	if ((flag = fcntl(fd, F_GETFL, 0)) == -1)
		myExit();
	if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
		myExit();

	//cout << "set " << fd << " non-block succeed" << endl;
}

bool writeFile(const ClientInfo & cinfo, bool isServer)
{
	char filename [40], filepath[50];
	sprintf(filename,"%d.%d.pid.txt",cinfo.msg.StuNo,cinfo.msg.clientPid);
    if(isServer){
        strcpy(filepath, "./serverout");
        if(mkdir(filepath, S_IRWXU | S_IRWXG | S_IRWXO) == 0){
            cerr << "make directory failed!" <<endl;
            // exit(-1);
        }
        strcat(filepath, "/");
        strcat(filepath, filename);
    }
    else{
        strcpy(filepath, "./clientout");
        if(mkdir(filepath, S_IRWXU | S_IRWXG | S_IRWXO) == 0){
            cerr << "make directory failed!" <<endl;
            //exit(-1);
        }
        strcat(filepath, "/");
        strcat(filepath, filename);
    }

	ofstream fout ;
	fout.open(filepath,ios::out|ios::trunc);
	if(!fout.is_open()){
		cerr<<"open file failed !\n";
		return false ;
	}
	fout<<cinfo.msg.StuNo<<endl;
	fout<<cinfo.msg.clientPid<<endl;
	fout<<cinfo.msg.clientTime<<endl;
	
	for (int i =0 ;i<cinfo.msg.randomMsgLen;i++)
		fout<<cinfo.msg.randomMsg[i];
	
	//fout<<cinfo.msg.randomMsg<<endl;
	cout<<filepath<<" ready write"<<endl;
	fout.close();
	return true;
}


bool testConnect(const int cfd)
{
	static char msgTest[5];
	if (recv(cfd, msgTest, 5, MSG_PEEK) <= 0)
	{
		cerr << "client " << cfd << "dis connect " << endl;
		return false ;
	}
	return true ;

}

int parseCMD(int argc, char **argv ,int isClient )
{
    flag_block = false;
    flag_fork = false;
    port = -1;
    max_num = 100;

    bool hasip = (isClient == 0);

    int i ;
    for (i = 1; i<argc ; i++)
    {
        if (strstr(argv[i], "ip"))
        {
            server_addr.sin_addr.s_addr = inet_addr(argv[i + 1]);
            hasip = true; 
            i++;
        }

        else if (strstr(argv[i], "port"))
        {
            if(i>=argc-1){
                fprintf(stderr,"please input port !\n");
                exit(-1);
            }
            port = atoi(argv[i + 1]);
            i++;
        }

        else if (strstr(argv[i], "num"))
        {
            if(!isClient)
            {
                cerr<<"input num error !\n";
                exit(-1);
            }

            max_num = atoi(argv[i + 1]);
            i++;
        }

        else if (strcmp(argv[i] + 2, "block") == 0)
        {
            flag_block = true;
        }

        else if (strcmp(argv[i] + 2, "fork") == 0)
        {
            flag_fork = true;
        }

        else if (strcmp(argv[i] + 2, "nonblock") == 0)
        {
        }

        else if (strcmp(argv[i] + 2, "nofork") == 0)
        {
        }


        else
        {
            cerr<<"input error !\n";
            exit(-1);
        }

    }

    if(port == -1)
    {
        fprintf(stderr,"please input port !\n");
        exit(-1);
    }

    //if (port == -1)
     //   return false;

    if(flag_block && !flag_fork)
        flag_block = false ;

    if(!hasip){
        fprintf(stderr,"please input ip !\n");
        exit(-1);
    }
    server_addr.sin_port = htons(port);
    server_addr.sin_family=AF_INET;
    return true;
}


