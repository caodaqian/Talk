#include<iostream>
#include<sys/socket.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
using namespace std;

static int _sock_count = 0;

int SocketCreate(char *ip, int port)
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        cerr<<"socket create fail; errno:"<<errno<<endl;
    }

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);

    int res = connect(sockfd,(struct sockaddr *)&saddr, sizeof(saddr));
    if(res == -1)
    {
        return -1;
    }
    else
    {
        _sock_count++;
    }
    return sockfd;
}

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        cout << "arg isn't enough! (ip,port)" << endl;
        exit(1);
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);

    int recvSock = 0;
    while(1)
    {
        recvSock = SocketCreate(ip,port);
        if(recvSock == -1)
            break;
    }

    cout << "最大连接量：" << _sock_count << endl;
    return 0;
}
