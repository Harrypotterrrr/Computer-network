#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include <time.h>
#include <fstream>
#define FIFO_NAME "./fifo_to_physical"
#define BUF_LEN PIPE_BUF



int main()
{

	srand(time(0));
	using namespace std;

	ofstream fout("send1_data",ios::out);
	char cf;
	for(int i =0 ;i<1024*1024*23;i++)
	{
		cf = char(rand()%256);
		fout<<cf;
	}

	fout.close();
	
    
}