#include<iostream>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include <unistd.h>

using namespace std;

#define BACKLOG 50
#define MAXBUF 1024

//If successful, inetlisten sets a socket to listen mode and returns the listening file descriptor. 
//If inetlisten fails, it returns -1.
int inetlisten(const char *portnum){

    struct addrinfo *hints;
    struct addrinfo *result,*rp;
    int lfd = 0,oprval = 1;

    hints = new struct addrinfo;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_family = AF_UNSPEC;
    hints->ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    hints->ai_canonname = NULL;
    hints->ai_addr = NULL;
    hints->ai_next = NULL;

    if(getaddrinfo(NULL,portnum,hints,&result) != 0){
        free(hints);
        return -1;
    }
    for(rp = result;rp!=NULL;rp = rp->ai_next){
        lfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if(lfd == -1)
            continue;
        if(setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&oprval,sizeof(oprval)) == -1){
            close(lfd);
            free(hints);
            return -1;
        }
        if(bind(lfd,rp->ai_addr,rp->ai_addrlen) == 0)
            break;
        else{
            if(rp->ai_next == NULL){
                close(lfd);
                free(hints);
                freeaddrinfo(result);
                return -1;
            }
        }
        close(lfd);
    }
    free(hints);
    freeaddrinfo(result);

    if(listen(lfd,BACKLOG) == -1)
        return -1; 

    return lfd;
}

//If successful, inetconnect sets a socket to connect service and returns the connecting file descriptor. 
//If innetconnect fails, it returns -1.
int inetconnect(char *service,const char *portnum){

    struct addrinfo *hints;
    struct addrinfo *result,*rp;
    int cfd = 0,chk = 0;

    hints = new struct addrinfo;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_family = AF_UNSPEC;
    hints->ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    hints->ai_canonname = NULL;
    hints->ai_addr = NULL;
    hints->ai_next = NULL;

    if(getaddrinfo(service,portnum,hints,&result) != 0){
        free(hints);
        return -1;
    }
    for(rp = result;rp!=NULL;rp=rp->ai_next){
        cfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if(cfd == -1)
            continue;
        if(connect(cfd,rp->ai_addr,rp->ai_addrlen) != -1){
            chk = 1;
            break;
        }
        close(cfd);
    }
    free(hints);
    freeaddrinfo(result);

    if(chk == 0)
        return -1;

    return cfd;
}

//this function can transform address
//if function fails , it returns NULL
char *addressStr(char *result,ssize_t resultlen,sockaddr *addr,socklen_t addrlen){

    char host[MAXBUF],service[MAXBUF];
    if(getnameinfo(addr,addrlen,host,MAXBUF,service,MAXBUF,NI_NUMERICHOST) == 0){
        snprintf(result,resultlen,"(%s,%s)",host,service);
    }
    else{
        cout << "UNKNOWN?" << endl;
        return NULL;
    }
    return result;
}