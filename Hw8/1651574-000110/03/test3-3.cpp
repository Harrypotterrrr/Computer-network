#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#include <signal.h>

int main(){
    daemon(1,1);

    
    while(true){
        sleep(5);
        std::cout<< "the program is waiting..." <<std::endl;
    }
    return 0;
}