#include<iostream>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include"Threadpool.hpp"
#include"inetsockets_tcp.hpp"

using namespace std;

#define PORT_NUM "50000"
#define BACKLOG 50

struct addrinfo *hints;
struct addrinfo *result,*rp;
char recvbuf[MAXBUF]="";
char sendbuf[MAXBUF]=""; 
int cfd = 0,numread = 0;

int main(){

    cfd = inetconnect(NULL,PORT_NUM);
    if(cfd == -1){
        perror("innetconnect");
        return 0;
    };

    numread = read(cfd,recvbuf,MAXBUF);
    cout << recvbuf << endl;

    free(hints);
    freeaddrinfo(result);
    close(cfd);
    return 0;

}