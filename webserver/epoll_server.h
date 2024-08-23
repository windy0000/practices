#ifndef _EPOLL_SERVER_H
#define _EPOLL_SERVER_H

void Accept(int cfd,int epfd);
void disconnect(int cfd,int epfd);
void http_request(int cfd,const char *file);
void Read(int cfd,int epfd);
int init_lfd(int port, int epfd);
void epoll_run(int port);

#endif