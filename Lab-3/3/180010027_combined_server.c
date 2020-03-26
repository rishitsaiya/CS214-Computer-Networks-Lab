#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <errno.h> 
#include <netdb.h>
#include <signal.h> 

// #define PORT 50027 
#define MAXLINE 1024 
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 


int main(int argc, char** argv) 
{ 

    if(argc < 2){
        printf("Enough arguments not provided.\n");
        exit (0);
    }

    int PORT = atoi(argv[1]);

    int listenfd, connfd, udpfd, nready, maxfdp1; 
    char buff[MAXLINE]; 
    pid_t childpid; 
    fd_set rset; 
    ssize_t n; 
    socklen_t len; 
    const int on = 1; 
    struct sockaddr_in cliaddr, servaddr; 
    // char* message = "Hello Client"; 
    void sig_chld(int); 
  
    /* create listening TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0); 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // binding server addr structure to listenfd 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    listen(listenfd, 10); 
  
    /* create UDP socket */
    udpfd = socket(AF_INET, SOCK_DGRAM, 0); 
    // binding server addr structure to udp sockfd 
    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
  
    // clear the descriptor set 
    FD_ZERO(&rset); 
  
    // get maxfd 
    maxfdp1 = max(listenfd, udpfd) + 1; 
    for (;;) { 
  
        // set listenfd and udpfd in readset 
        FD_SET(listenfd, &rset); 
        FD_SET(udpfd, &rset); 
  
        // select the ready descriptor 
        nready = select(maxfdp1, &rset, NULL, NULL, NULL); 
  
        // if tcp socket is readable then handle 
        // it by accepting the connection 
        if (FD_ISSET(listenfd, &rset)) { 
            len = sizeof(cliaddr); 
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len); 
            if ((childpid = fork()) == 0) { 
                close(listenfd); 
                bzero(buff, sizeof(buff)); 
                time_t now;
                time(&now);
                strcpy(buff, ctime(&now));
                printf("Request From TCP client at %s", buff); 
                write(connfd, (const char*)buff, sizeof(buff)); 
                close(connfd); 
                exit(0); 
            } 
            close(connfd); 
        } 
        // if udp socket is readable receive the message. 
        if (FD_ISSET(udpfd, &rset)) { 
            len = sizeof(cliaddr); 
            bzero(buff, sizeof(buff)); 
            printf("\nMessage from UDP client: "); 
            n = recvfrom(udpfd, buff, sizeof(buff), 0, 
                         (struct sockaddr*)&cliaddr, &len); 
            puts(buff); 
            buff[n-1]='\0';

           struct hostent *host_info = gethostbyname(buff);
           if(host_info == NULL)
           {
                printf("NULL\n");
                strcpy(buff, "Error!!");
                sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
                return(-1);
           }

           if(host_info->h_addrtype == AF_INET)
           {
                struct in_addr **address_list = (struct in_addr **)host_info->h_addr_list;
                for(int i = 0; address_list[i]!=NULL; i++)
                {
                    // bzero(buff, MAXLINE);
                    inet_ntop(AF_INET, address_list[i], buff, INET_ADDRSTRLEN);
                    printf("%s\n", buff);
                    sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
                }
           }
           else if(host_info->h_addrtype == AF_INET6)
           {
                struct in6_addr **address_list = (struct in6_addr **)host_info->h_addr_list;
                for(int i = 0; address_list[i]!=NULL; i++)
                {
                    // bzero(buff, MAXLINE);
                    inet_ntop(AF_INET6, address_list[i], buff, INET_ADDRSTRLEN);
                    printf("%s\n", buff);
                    sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
                }
           }
           bzero(buff, MAXLINE);
           strcpy(buff, "done");
           sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);

        } 
    } 
    return 0;
} 

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_combined_server.c -o server
        ./server 50027
*/

//------------------------------------------Output----------------------------------------------------------
/*
    Request From TCP client at Sun Feb  9 00:58:14 2020 - (When ran the time client)
    Message from UDP client: www.youtube.com - (When ran the dns client)

    216.58.197.46
    216.58.197.78
    172.217.26.174
    172.217.26.206
    172.217.167.142
    172.217.31.206
    172.217.160.142
    216.58.196.174
    172.217.163.46
    172.217.163.110
    172.217.163.142
    172.217.163.174
    172.217.163.206
    216.58.200.142
*/