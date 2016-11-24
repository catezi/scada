#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED



#endif // DEFINE_H_INCLUDED

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
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <malloc.h>
#include <queue>
#include <Python.h>
#include <codecvt>
#include <fstream>
#include <cstdio>
#include <sstream>

using namespace std;


#define BUFFER_SIZE 1024
#define HTTP_POST "POST /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n"\
    "Content-Type:application/octet-stream/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s"
#define HTTP_GET "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n\r\nContent-Type:application/octet-stream\r\n\r\n"
#define BUFFER_LENGTH 100
#define BUFFER_MAX 65535
#define ASIZE 256
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define ITEMMAX 1000
#define patternnum 4
#define ASCII 256
#define patternlength 100

struct target {
    char *host;
    int port;
};


struct ARGS {
   string filename;
   int threadid;
   bool finish;
};

typedef struct node {
    struct node *next[ASCII];
    struct node *par;
    struct node *fail;
    char inputchar[2];
    int patterTag;
    int patterNo;
}*Tree, TreeNode;


extern char pattern[patternnum][patternlength];
extern pthread_mutex_t mutex;





