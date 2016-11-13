#include "Headers/define.h"
#include "Headers/client.h"
#include "Headers/patternmatch.h"

using namespace std;


const int patternlength = 5;
extern char x[];
extern int bmGs[];
extern int bmBc[];
extern pthread_mutex_t mutex;
extern int currentfile;
extern int itemsum;
extern int filenum;

void *analysis(void * arg) {
        struct ARGS* args= (struct ARGS*) arg;
        //struct target *tar = args->arg1;
        struct target *tar = (struct target *)malloc(sizeof(struct target));
        cout << args->host << args->port << endl;
        tar->host = (char*)((args->host).c_str());
        tar->port = args->port;
        //int currentfile = args->arg2;
        //int itemsum = args->arg3;
        //int filenum = args->arg4;
        //struct target *tar = (struct target*) arg;
        cout << tar->host << tar->port << endl;
        int socket_fd = -1;
        int i = 0, j = 0;
        int messagelength = 0;
        struct hostent *he;
        struct sockaddr_in *server_addr;
        char buff[BUFFER_MAX] = {'\0'};
        char outfilename[10] = {'\0'};
        //cout << tar.host <<endl;
        socket_fd = http_tcpclient_create();
        cout << "tcpclient create success" << endl;
        if(socket_fd < 0){
            cout << "host: " << tar->host << " port: " << tar->port << "[**]http_tcpclient_create failed." << endl;
            return arg;
        }
        he = (struct hostent*)malloc(sizeof(struct hostent));
        server_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        if ((messagelength = http_get((*tar).host, (*tar).port, socket_fd, he, server_addr, buff)) == -1) {
            cout << "host: " << tar->host << " port: " << tar->port << "[**]http_get failed" << endl;
            return arg;
        }
        //cout << buff << endl;
        cout << messagelength << endl;
        cout << "http_get success" << endl;
        while (j <= messagelength - patternlength) {
            for (i = patternlength - 1; i >= 0 && x[i] == buff[i + j]; --i);
            if (i < 0) {
                cout << "HERE!" << endl;
                pthread_mutex_lock (&mutex);
                //cout << j << endl;
                currentfile ++;
                itemsum ++;
                if (currentfile > ITEMMAX) {
                    currentfile = 1;
                    filenum ++;
                }
                if (filenum == 1 && currentfile == 1) {
                    //cout << filenum << endl;
                    sprintf(outfilename, "%d.txt", filenum);
                    //cout <<outfilename << endl;
                    ofstream out(outfilename);
                    if (out.is_open()) {
                        out << "include scada:\n";
                        out.close();
                    }
                }
                char s[BUFFER_MAX] = {'\0'};
                sprintf(s, "%d. HOST: %s PORT: %d\nmessage:\n%s\n",currentfile, tar->host, tar->port, buff);
                sprintf(outfilename, "%d.txt", filenum);
                cout << outfilename << endl;
                ofstream out(outfilename, std::ios::app);
                if (!out.is_open()) {
                    open(outfilename, (O_CREAT|O_WRONLY|O_TRUNC));
                    ofstream out(outfilename, std::ios::app);
                }
                out << s;
                out.close();
                j += bmGs[0];

                pthread_mutex_unlock(&mutex);
                break;
            }
            else
                j += MAX(bmGs[i], bmBc[buff[i + j]] - patternlength + 1 + i);
        }

        //cout << buff << endl;
        free((void*)he);
        free((void*)server_addr);
        //messagelength = 0;
}
