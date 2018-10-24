#include <fstream>
#include "test.h"

int ReadFile(const char * path)
{
    ifstream infile(path, ios::in);
    if(infile == NULL){
        cerr<<"file cannot open!\n";
        exit(-1);
    }
    char buff[100];
    int num;
    while(infile.getline(buff,100)){
        if(strlen(buff) <= 11)
            num = 5;
        else{
            strcpy(buff,buff+11);
            num = atoi(buff);
        }
    }
    return num;
}
