#include "Headers/define.h"
#include "Headers/client.h"
#include "Headers/patternmatch.h"
#include "Headers/mythread.h"
#include "Headers/callpython.h"

char pattern[patternnum][patternlength] = {{'\0'}};
pthread_mutex_t mutex;


using namespace std;

long getCurrentTime()
{
   struct timeval tv;
   gettimeofday(&tv,NULL);
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


int main(int argc, char *argv[])
{
    pthread_t id[argc-1];
    pthread_mutex_init (&mutex,NULL);
    string filename[argc-1];
    struct ARGS args[argc-1];


    int originpatternlength = 20;
    string originpattern[patternnum] = {"scada", "SCADA", "scada", "SCADA"};

    for (int i = 0; i <= patternnum-1; i ++) {
        string s = transform(originpattern[i]);
        strcpy(pattern[i], s.c_str());
        cout << originpattern[i] << endl;
        cout << pattern[i] << endl;
    }


    for (int i = 1; i <= argc-1; i ++) {
        //pthread_t testid;
        filename[i-1] = argv[i];
        args[i-1].filename = filename[i-1];
        args[i-1].threadid = i;
        cout << args[i-1].filename << args[i-1].threadid << endl;
        //int res = pthread_create(&testid, NULL, mythread, NULL);
        int res = pthread_create(&id[i-1], NULL, mythread, (void *)&args[i-1]);
        if (res) {
            cout << "create thread failed!" << endl;
        }
        cout << getCurrentTime() << endl;
    }
    sleep(600);

}
