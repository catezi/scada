#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED



#endif // CLIENT_H_INCLUDED


int http_tcpclient_create();
int http_tcpclient_connect(int socket_fd, char* host, int port, struct hostent *he, struct sockaddr_in *server_addr);
void http_tcpclient_close(int socket);
int http_tcpclient_send(int socket,char *buff,int size);
int http_tcpclient_recv(int socket, char buff[]);
int http_post(char *host, int port, int socket_fd, char *post_str, struct hostent *he, struct sockaddr_in *server_addr, char buff[BUFFER_MAX]);
int http_get(char *host, int port, int socket_fd, struct hostent *he, struct sockaddr_in *server_addr, char buff[BUFFER_MAX]);
