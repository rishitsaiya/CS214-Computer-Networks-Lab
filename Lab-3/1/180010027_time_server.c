#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <time.h>

// #define PORTN 50027 

int main(int argc, char** argv)
{
    if(argc < 2){
        printf("Enough arguments not provided.\n");
        exit (0);
    }
    
    int PORTN = atoi (argv[1]);

    struct sockaddr_in address;
    struct sockaddr_in cli;int sockfd,conntfd;int len,ch;char str[100];
    time_t tick;
    
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    if(sockfd<0){
        printf("error in socket\n");
        exit(0);
    }
    else 
        printf("Socket opened.\n");
    
    bzero(&address,sizeof(address));
    address.sin_port=htons(PORTN);
    address.sin_addr.s_addr=htonl(0);
    
    if(bind(sockfd,(struct sockaddr*)&address,sizeof(address))<0){
        printf("Error in binding\n");
    }
    
    else
        printf("Binded Successfully.\n");
    
    listen(sockfd,50);
    
    while (1){
    // for(;;){
        len=sizeof(ch);
        conntfd=accept(sockfd,(struct sockaddr*)&cli,&len);
        printf("Accepted.\n");
        tick=time(NULL);
        snprintf(str,sizeof(str),"%s",ctime(&tick));
        printf("Date and time sent.\n");
        write(conntfd,str,100);
    }

return 0;
}

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_time_server.c -o server
        ./server 50027
*/

//------------------------------------------Output----------------------------------------------------------
/*
    Socket opened.
    Binded Successfully.
    Accepted.
    Date and time sent.
*/