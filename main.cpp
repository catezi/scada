#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


#define BUFFER_SIZE 1024
#define HTTP_POST "POST /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n"\
    "Content-Type:application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s"
#define HTTP_GET "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n\r\n"
#define BUFFER_LENGTH 100
#define BUFFER_MAX 65535
#define ASIZE 256
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define ITEMMAX 1000

int patternlength = 0;
int messagelength = 0;

using namespace std;

struct target {
    char *host;
    int port;
};
/**
    create client
*/
static int http_tcpclient_create(){
    int socket_fd;
    if((socket_fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        return -1;
    }
    return socket_fd;
}
/**
    connect server
*/
static int http_tcpclient_connect(int socket_fd, char* host, int port, struct hostent *he, struct sockaddr_in *server_addr) {
    cout << "http_tcpclient_connect start" << endl;
    if (socket_fd == -1) {
        return -1;
    }
    if((he = gethostbyname(host))==NULL){
        return -1;
    }
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr = *((struct in_addr *)he->h_addr);
    if(connect(socket_fd, (struct sockaddr *)server_addr,sizeof(struct sockaddr)) == -1){
        return -1;
    }
    cout << "http_tcpclient_connect finish" << endl;
    return socket_fd;
}
/**
    close connection
*/
static void http_tcpclient_close(int socket){
    close(socket);
}

/**
    send message
*/
static int http_tcpclient_send(int socket,char *buff,int size){
    int sent=0,tmpres=0;
    cout << "http_tcpclient_send start" << endl;
    while(sent < size){
        tmpres = send(socket,buff+sent,size-sent,0);
        if(tmpres == -1){
            return -1;
        }
        sent += tmpres;
    }
    cout << "httptcp_client_send finish" << endl;
    return sent;
}


/**
    receive message
*/
static int http_tcpclient_recv(int socket, char buff[]){
    int i = 0;
    int res = 0;
    cout << "http_tcpclient_recv start" << endl;
    char lpbuff[BUFFER_SIZE*4] = {'\0'};
    while ((res = recv(socket, lpbuff, BUFFER_SIZE*4, 0)) > 0) {
        strcat(buff, lpbuff);
        messagelength += res;
        i ++;
        lpbuff[BUFFER_SIZE*4] = {'\0'};
        if (res < BUFFER_SIZE*4) {
            break;
        }
    }
    if (i == 0) {
        return -1;
    }
    cout << "http_tcpclient_recv finish" << endl;
    return i;
}


int http_post(char *host, int port, int socket_fd, char *post_str, struct hostent *he, struct sockaddr_in *server_addr, char buff[BUFFER_MAX]){

    char file[BUFFER_SIZE] = {'\0'};
    char sendbuf[BUFFER_SIZE*4] = {'\0'};
    if(!host || !post_str){
        printf("[**]failed!\n");
        return -1;
    }
    sprintf(sendbuf, HTTP_POST, file, host, port, strlen(post_str), post_str);

    if(http_tcpclient_connect(socket_fd, host, port, he, server_addr) == -1) {
        return -1;
    }

    if(http_tcpclient_send(socket_fd, sendbuf, strlen(sendbuf)) < 0){
        printf("http_tcpclient_send failed..\n");
        return -1;
    }

    if (http_tcpclient_recv(socket_fd, buff) == -1) {
        cout << "http_tcpclient_recv failed.." << endl;
        return -1;
    }


    return 0;
}

int http_get(char *host, int port, int socket_fd, struct hostent *he, struct sockaddr_in *server_addr, char buff[BUFFER_MAX])
{
    char file[BUFFER_SIZE] = {'\0'};
    char sendbuf[BUFFER_SIZE*4] = {'\0'};
    cout << "http_get start" << endl;
    if(!host){
        printf("      failed!\n");
        return -1;
    }
    if(http_tcpclient_connect(socket_fd, host, port, he, server_addr) == -1) {
        return -1;
    }
    sprintf(sendbuf, HTTP_GET, file, host, port);
    if(http_tcpclient_send(socket_fd, sendbuf, strlen(sendbuf)) < 0){
        printf("http_tcpclient_send failed..\n");
        return -1;
    }
    if (http_tcpclient_recv(socket_fd, buff) == -1) {
        cout << "http_tcpclient_recv failed.." << endl;
        return -1;
    }
    cout << "http_get finish" << endl;
    return 0;
}

void preBmBc(char *x, int m, int bmBc[]) {
   int i;
   for (i = 0; i < ASIZE; ++i)
      bmBc[i] = m;
   for (i = 0; i < m - 1; ++i)
      bmBc[x[i]] = m - i - 1;
}


void suffixes(char *x, int m, int *suff) {
   int f, g, i;
   suff[m - 1] = m;
   g = m - 1;
   for (i = m - 2; i >= 0; --i) {
      if (i > g && suff[i + m - 1 - f] < i - g)
         suff[i] = suff[i + m - 1 - f];
      else {
         if (i < g)
            g = i;
         f = i;
         while (g >= 0 && x[g] == x[g + m - 1 - f])
            --g;
         suff[i] = f - g;
      }
   }
}

void preBmGs(char *x, int m, int bmGs[]) {
   int i, j, suff[patternlength];
   suffixes(x, m, suff);
   for (i = 0; i < m; ++i)
      bmGs[i] = m;
   j = 0;
   for (i = m - 1; i >= 0; --i)
      if (suff[i] == i + 1)
         for (; j < m - 1 - i; ++j)
            if (bmGs[j] == m)
               bmGs[j] = m - 1 - i;
   for (i = 0; i <= m - 2; ++i)
      bmGs[m - 1 - suff[i]] = m - 1 - i;
}



int main(int argc, char *argv[])
{
    int socket_fd = -1;
    int currentfile = 0;
    int itemsum = 0;
    int filenum = 1;
    char x[] = {"scada"};
    patternlength = strlen(x);
    int bmGs[patternlength]={0}, bmBc[ASIZE]={0};
    struct target tar;
    struct hostent *he;
    struct sockaddr_in *server_addr;
    char *infilename;
    char outfilename[10] = {'\0'};
    if (argc == 2) {
        infilename = argv[1];
    }
    else {
        cout << "[**]please input only one arguments." << endl;
        return 0;
    }
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
        if (http_get(tar.host, tar.port, socket_fd, he, server_addr, buff) == -1) {
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
                    ofstream out(outfilename);
                    if (out.is_open()) {
                        out << "";
                        out.close();
                    }
                }
                char s[BUFFER_MAX] = {'\0'};
                sprintf(s, "%d. HOST: %s PORT: %d\nmessage:\n%s\n",currentfile, tar.host, tar.port, buff);
                sprintf(outfilename, "%d.txt", filenum);
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

        cout << buff << endl;
        free((void*)he);
        free((void*)server_addr);
        messagelength = 0;
    }
    http_tcpclient_close(socket_fd);
    in.close();


    return 0;
}
