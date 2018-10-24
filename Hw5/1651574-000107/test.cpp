#include "test.h"

priority_queue<int, vector<int>, greater<int> >pQ;

SubProcess *sp[30];
int* ctr_subprocess;
int MaxSubProcess;


char **new_environ, **new_argv;
int len_env;

void SetShortName(int no, int argc, char** argv)
{
    if (no != 0)
        sprintf(argv[0],"./test [%s]",sp[no]->name);
    else
        sprintf(argv[0],"./test [main]");
    prctl(PR_SET_NAME, argv[0]);
    // cout<<"short name"<< argv[0]<<endl;

    // stringstream tmp;

    // if(no != -1)
    //     // tmp << new_argv[0] <<" ["<<sp[no]->name<<"]";    
    //     tmp << "./test" <<" "<<sp[no]->name<<"]";    
    // else
    //     tmp << new_argv[0] <<" [main]";
    // tmp >>argv[0];

    // prctl(PR_SET_NAME, argv[0]);
}

void SetLongName(int no, int argc, char** argv)
{
    if(no != 0)
        sprintf(argv[0],"./test [%s %s]", sp[no]->name, sp[no]->timeString);
    else
        sprintf(argv[0],"./test [main %s]", sp[no]->timeString);

    // cout<<"long name"<< argv[0]<<endl;
    // stringstream tmp;
    // char tmp_show_long[100];
    // tmp << "./test ["<<sp[no]->name<<" "<<sp[no]->timeString<<"]";
    // tmp >> argv[0];
}

void RespondKill(int sig)
{
    pid_t pid;

    while( (pid = waitpid(-1, NULL, WNOHANG)) > 0 ) {
        (*ctr_subprocess)--;
        int findNo = -1;
        for(int i=1 ; i<=MaxSubProcess ; i++)
            if(sp[i]->pid == pid){
                findNo = i;
                break;
            }

        if (findNo == -1){
            cerr<<"find pid ERROR!"<<endl;
            exit(-1);
        }
        pQ.push(findNo);
        sp[findNo]->Reset();
    }
    return;
}

void RestoreProcessSpace(int argc, char** argv)
{
    stringstream tmp;
    for(int i=0; i<argc; i++){
        tmp << new_argv[i];
        tmp >> argv[i];
    }
    for(int i=0; i < len_env ; i++){
        tmp << new_environ[i];
        tmp >> environ[i];
    }
}

void BackUpProcessSpace(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
        new_argv[i] = strdup(argv[i]);

    len_env = sizeof(*environ) / sizeof(**environ);
    if (environ) {
        unsigned int i = -1;
        while (environ[++i])
            new_environ[i] = strdup(environ[i]);
    }
}

void Init(int argc, char** argv)
{
    new_environ = new char*[100];
    new_argv = new char*[100];
    // BackUpProcessSpace(argc, argv);
    // RestoreProcessSpace(argc, argv);

    for(int i=1 ; i<=MaxSubProcess ; i++)
        pQ.push(i);

    int shmID[30], ctrID;

    ctrID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0600);
    if(ctrID < 0){
        cerr<<"get shared memory ID ERROR"<<endl;
        exit(-1);
    }
    ctr_subprocess = (int*)shmat(ctrID, NULL, 0);

    for(int i=0 ; i<=MaxSubProcess ; i++){
        shmID[i] = shmget(IPC_PRIVATE, sizeof(SubProcess), IPC_CREAT|0600);
        if(shmID[i] < 0){
            cerr<<"get shared memory ID ERROR"<<endl;
            exit(-1);
        }
        sp[i] = (SubProcess*)shmat(shmID[i], NULL, 0);
        sp[i]->Reset();
    }
}

int SelectSubNo()
{
    if(pQ.empty()){
        cerr<<"Runtime ERROR!"<<endl;
        exit(-1);
    }
    int rtn = pQ.top();
    pQ.pop();
    return rtn;
}


void SubprocessFunc(int no, int argc, char** argv)
{
    prctl(PR_SET_PDEATHSIG, SIGTERM);
    (*ctr_subprocess)++;
    sp[no]->SetName(no);
    sp[no]->pid = getpid();

    SetShortName(no, argc, argv);
    while(true){
        SetLongName(no, argc, argv);
        sleep(1);
        sp[no]->RefreshRunTime();
    }
}
void MainprocessFunc(int no, int argc, char** argv)
{
    SetLongName(0, argc, argv);
    sp[0]->RefreshRunTime();
}

int main(int argc, char **argv)
{
    pid_t pid,ppid;

    pid = fork();

    if (pid < 0) {
        cerr<<"Create subprocess ERROR!\n";
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        cerr<< "daemon process create success!\n";
        exit(EXIT_SUCCESS);
    }
    umask(0);
    chdir("/");

    MaxSubProcess = ReadFile(path);
    Init(argc, argv);
    if(MaxSubProcess > 30){
        cerr<<"the maximal number of the proces is overflowed!"<<endl;
        exit(-1);
    }

    signal(SIGCHLD, &RespondKill);

    SetShortName(0, argc, argv);

    while(true){
        while((*ctr_subprocess) != MaxSubProcess){
            int no = SelectSubNo();
            pid = fork();
            if(pid < 0){
                cerr<<"Create new subprocess error!"<<endl;
                exit(-1);
            }
            if(pid == 0){
                SubprocessFunc(no, argc, argv);
                exit(0);
            }
            if (pid > 0){
                MainprocessFunc(0, argc, argv);
            }
            sleep(1);
        }
        MainprocessFunc(0, argc, argv);
        sleep(1);
    }
    return 0;
}