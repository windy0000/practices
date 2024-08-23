#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>
#include "util.h"

#define MAX_EVENT 128
void Accept(int lfd,int epfd)
{
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    int cfd = accept(lfd,(struct sockaddr*)&client_addr,&client_addr_len);
    if(cfd==-1)     sys_error("accept error");
    // 打印客户端IP+port  
    char client_ip[64] = {0};
    printf("New Client IP: %s,Port:%d,cfd=%d\n",\
    inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),\
    ntohs(client_addr.sin_port),cfd);

    int flag = fcntl(cfd,F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd,F_SETFL,flag);

    // 挂树上
    struct epoll_event ev;
    ev.data.fd = cfd;
    ev.events = EPOLLIN | EPOLLET;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
    if(ret==-1)     sys_error("epoll_ctl add cfd error");
}
void disconnect(int cfd,int epfd)
{
    printf("客户端断开了连接...\n");

    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,cfd,NULL);
    if(ret==-1)     sys_error("epoll_ctl del cfd error");
    close(cfd);
}
void http_request(int cfd,const char *file)
{   
    // 文件是否存在
    struct stat sbuf;
    int ret = stat(file,&sbuf);
    if(ret == -1)
    {
        send_error(cfd, 404, "Not Found", "NO such file or direntry");
        return;
    }

    if(S_ISDIR(sbuf.st_mode))
    {
        send_respond_head(cfd, 200, "OK", get_file_type(".html"), -1);
        send_dir(cfd, file);
    }else if(S_ISREG(sbuf.st_mode))
    {
        send_respond_head(cfd,200,"OK",get_file_type(file),sbuf.st_size);
        send_file(cfd,file);
    }
}
void Read(int cfd,int epfd)
{       // 读取一行http协议， 拆分， 获取 get 文件名 协议号

    // 读取
    char line[1024] = {0};
    int len = get_line(cfd,line,sizeof(line));
    if(len==0)  disconnect(cfd,epfd);
    else{
        while(1)
        {
            char buf[1024] = {0};
            len = get_line(cfd,buf,sizeof(buf));

            if(buf[0]=='\n'||len==-1)   break;
        }
    }

    // 拆分(目前只处理GET，不找POST)
    if(strncasecmp("GET",line,3)==0)
    {
        char method[12], path[1024], protocol[12];
        sscanf(line,"%[^ ] %[^ ] %[^ ]",method,path,protocol);
        printf("method = %s, path = %s, protocol = %s\n", method, path, protocol);
        char *file = path + 1;  // 只能是/xx文件形式
        if(strcmp(path,"/")==0) file = "./";

        http_request(cfd,file); // 主要是返回文件

        disconnect(cfd,epfd);
    }

}
int init_lfd(int port, int epfd)
{
    int ret;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd<0)   sys_error("socket create error");

    // 端口复用
    int flag = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    ret = bind(lfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(ret==-1)     sys_error("bind error");
    ret = listen(lfd,64);
    if(ret==-1)     sys_error("listen error");

    struct epoll_event tmp;
    tmp.events = EPOLLIN;
    tmp.data.fd = lfd;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&tmp);
    if(ret==-1)
        sys_error("epoll_ctl add lfd error");

    return lfd;
}
void epoll_run(int port)
{
    int epfd = epoll_create(1);
    if(epfd==-1)     sys_error("create epoll error");

    // lfd监听并挂上树
    int lfd = init_lfd(port,epfd);

    struct epoll_event events[MAX_EVENT];
    while(1)
    {
        int wait_ret = epoll_wait(epfd,events,MAX_EVENT,0);
        if(wait_ret==-1)    sys_error("epoll wait error");

        struct epoll_event tmp;
        for(int i=0;i<wait_ret;i++)
        {
            tmp = events[i];
            if(!(tmp.events&EPOLLIN))  continue;

            int cfd = tmp.data.fd;
            if(cfd==lfd)    Accept(lfd,epfd);
            else            Read(cfd,epfd);
        }
    } 

    return 0;
}