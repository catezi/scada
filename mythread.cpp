#include "Headers/define.h"
#include "Headers/client.h"
#include "Headers/patternmatch.h"
#include "Headers/analysis.h"

const int patternlength = 5;


void* mythread(void * arg) {
    int socket_fd = -1;
    int currentfile = 0;
    int itemsum = 0;
    int filenum = 1;
    int messagelength = 0;
    char x[] = {"scada"};
 //   patternlength = strlen(x);
    int bmGs[patternlength]={0}, bmBc[ASIZE]={0};
    struct target tar;
    struct hostent *he;
    struct sockaddr_in *server_addr;
    struct ARGS *args = (struct ARGS *)arg;
    char *infilename;
    infilename = (char *)((args->filename).c_str());
    cout << "HELLO!" << endl;
    cout << infilename << endl;
    char outfilename[10] = {'\0'};
    ifstream in(infilename);
    if (!in.is_open()) {
        cout << "[**] please modify the file's name." << endl;
        return 0;
    }
    preBmGs(x, patternlength, bmGs);
    preBmBc(x, patternlength, bmBc);

    while (!in.eof()) {
        int i = 0, j = 0;
        char line[256] = {'\0'};
        char host[20] = {'\0'};
        tar.port = 0;
        in.getline(line, 100);
        if (line[0] == '\0') {
            break;
        }
        for (i = 0; line[i] != ' '; i ++) {
            host[i] = line[i];
        }
        tar.host = host;
        for (i ++; line[i] != '\0'; i ++) {
            tar.port = tar.port*10 + (line[i] - '0');
        }
        char buff[BUFFER_MAX] = {'\0'};
        cout << tar.host <<endl;
        socket_fd = http_tcpclient_create();
        cout << "tcpclient create success" << endl;
        if(socket_fd < 0){
            cout << "host: " << tar.host << " port: " << tar.port << "[**]http_tcpclient_create failed." << endl;
            continue;
        }
        he = (struct hostent*)malloc(sizeof(struct hostent));
        server_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        if ((messagelength = http_get(tar.host, tar.port, socket_fd, he, server_addr, buff)) == -1) {
            cout << "host: " << tar.host << " port: " << tar.port << "[**]http_get failed" << endl;
            continue;
        }
        cout << "http_get success" << endl;
        while (j <= messagelength - patternlength) {
            for (i = patternlength - 1; i >= 0 && x[i] == buff[i + j]; --i);
            if (i < 0) {
                cout << j << endl;
                currentfile ++;
                itemsum ++;
                if (currentfile > ITEMMAX) {
                    currentfile = 1;
                    filenum ++;
                }
                if (filenum == 1 && currentfile == 1) {
                    sprintf(outfilename, "%d_%d.txt", args->threadid, filenum);
                    cout << outfilename << endl;
                    ofstream out(outfilename);
                    if (out.is_open()) {
                        out << "include scada:\n";
                        out.close();
                    }
                }
                char s[BUFFER_MAX] = {'\0'};
                sprintf(s, "%d. HOST: %s PORT: %d\nmessage:\n%s\n",currentfile, tar.host, tar.port, buff);
                sprintf(outfilename, "%d_%d.txt", args->threadid, filenum);
                cout << outfilename << endl;
                ofstream out(outfilename, std::ios::app);
                if (!out.is_open()) {
                    open(outfilename, (O_CREAT|O_WRONLY|O_TRUNC));
                    ofstream out(outfilename, std::ios::app);
                }
                out << s;
                out.close();
                j += bmGs[0];
                break;
            }
            else
                j += MAX(bmGs[i], bmBc[buff[i + j]] - patternlength + 1 + i);
        }

        //cout << buff << endl;
        free((void*)he);
        free((void*)server_addr);
        messagelength = 0;
    }
    http_tcpclient_close(socket_fd);
    in.close();
    return arg;
}
