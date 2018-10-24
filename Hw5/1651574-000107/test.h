#pragma once
// #ifndef _test_h
// #define _test_h 

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <set>
#include <cstring>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
using namespace std;


class SubProcess{
public:
    int runtime;
    int pid;
    char timeString[50];
    char name[10];

    void RefreshRunTime();
    void RefreshtimeString();
    void SetName(int no);
    void Reset();
};

extern priority_queue<int, vector<int>, greater<int> >pQ;
extern int *ctr_subprocess;
extern int MaxSubProcess;
extern char** environ;


const char SUB[4] = "sub";
const char path[50]="/etc/1651574.conf";

int ReadFile(const char*);

// #endif
