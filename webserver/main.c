#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#include "epoll_server.h"

int main(int argc,char *argv[])
{
    if(argc<3)
        sys_error("To running this program:\n ./server port workpath\n");


    int port = atoi(argv[1]);
    char *path = argv[2];
    
    int ret = chdir(path);
    if(ret==-1)     sys_error("chdir error");


    epoll_run(port);

    return 0;    
}