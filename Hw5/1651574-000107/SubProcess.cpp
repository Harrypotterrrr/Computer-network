#include "test.h"

void MyInt2String(int no, char *s, int flag)   //*s ->*&s
{
    stringstream tmp;
    int i = 0, len;
    do{
        s[i] = no % flag + '0';
        no /= flag;
        i++;
    }while(no != 0);
    s[i] = '\0';
    len = strlen(s);
    for(i=0 ; i<len/2 ; i++){
        char t = s[i];
        s[i] = s[len-1-i];
        s[len-i-1] = t;
    }
    tmp << setw(2) << setfill('0') << s;
    tmp >> s;
}

struct MTime{
    char hour[4];
    char min[4];
    char sec[4];
    MTime(int h=0, int m=0, int s=0){
        MyInt2String(h, hour, 10);
        MyInt2String(m, min, 10);
        MyInt2String(s, sec, 10);
    }
};

void SubProcess::Reset(){
    pid = 0;
    runtime = 0;
    timeString[0] = name[0] = '\0';
    RefreshtimeString();
}

void SubProcess::SetName(int no){
    char no_s[50];
    MyInt2String(no,no_s,10);
    // cout<<"no_s:"<<no_s<<endl;
    strcpy(this->name, SUB);
    strcat(this->name, "-");
    strcat(this->name, no_s);
}

void SubProcess::RefreshRunTime(){
    this->runtime ++;
    this->RefreshtimeString();
}
void SubProcess::RefreshtimeString(){
    MTime mt = MTime(this->runtime/3600, this->runtime / 60 % 60, this->runtime % 60);
    strcpy(timeString, mt.hour);
    strcat(timeString, ":");
    strcat(timeString, mt.min);
    strcat(timeString, ":");
    strcat(timeString, mt.sec);
}
