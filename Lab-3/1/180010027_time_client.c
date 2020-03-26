#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 

// #define PORT 50027 

int main(int argc, char** argv)
{
    if(argc < 3){
        printf("Enough arguments not provided.\n");
        exit (0);
    }

    int PORTN = atoi (argv[2]);
    struct sockaddr_in address;
    int n,sockfd;
    int len;char buff[100];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    if(sockfd<0){ 
        printf("\nError in Socket");
        exit(0);
    }
    
    else printf("Socket is Opened.\n");
    
    bzero(&address,sizeof(address));
    address.sin_family=AF_INET;
    address.sin_port=htons(PORTN);

    if(inet_pton(AF_INET, argv[1], &address.sin_addr) <= 0){
        printf ("Invalid Address.\n");
        return -1;
    }


    if(connect(sockfd,(struct sockaddr*)&address,sizeof(address))<0){
        printf("\nError in connection failed");
        exit(0);
    }
    
    else{
        printf("Connected successfully.\n");
        if(n=read(sockfd,buff,sizeof(buff))<0){
            printf("\nError in Reading");
            exit(0);
        }
        
        else{
            printf("The local date and time is: %s", buff);
        }
    }

return 0;
}

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_time_client.c -o client
        ./client 0.0.0.0 50027
*/

//------------------------------------------Output----------------------------------------------------------
/*
    Socket is Opened.
    Connected successfully.
    The local date and time is: Sun Feb  9 00:39:53 2020

*/