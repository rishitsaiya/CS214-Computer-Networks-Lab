#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>

// #define PORT 50027 
#define MAXLINE 1024 

// Driver code 
int main(int argc, char** argv) { 
	if(argc < 2){
        printf("Enough arguments not provided.\n");
        exit (0);
    }

	int PORT = atoi(argv[1]);
	int sockfd; 
	char buff[MAXLINE]; 
	// char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	
	while(1) {

		int len, n; 

		len = sizeof(cliaddr); //len is value/resuslt 
		
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
       	// if (strcmp(buff, "exit\n") == 0){
		// 	exit(1);
		// }
		buff[n-1] = '\0'; 
		struct hostent *host_info = gethostbyname(buff);

       	if(host_info == NULL){
            // printf("DNS not found.\n");
            strcpy(buff, "Error!!");
            sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            return(-1);
       	}

       	if(host_info->h_addrtype == AF_INET){
            struct in_addr **address_list = (struct in_addr **)host_info->h_addr_list;
            for(int i = 0; address_list[i]!=NULL; i++){
                inet_ntop(AF_INET, address_list[i], buff, INET_ADDRSTRLEN);
                printf("%s\n", buff);
                sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            }
       	}

       	else if(host_info->h_addrtype == AF_INET6){
            struct in6_addr **address_list = (struct in6_addr **)host_info->h_addr_list;
            for(int i = 0; address_list[i]!=NULL; i++){
                // bzero(buff, MAXLINE);
                inet_ntop(AF_INET6, address_list[i], buff, INET_ADDRSTRLEN);
                printf("%s\n", buff);
                sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
            }
       	}

	bzero (buff, MAXLINE);
	strcpy(buff, "done");	
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 

		// printf("Client : %s\n", buff); 
		// printf("Hello message sent.\n"); 
	
	}
	
	return 0; 
}

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_dns_server.c -o server
        ./server 50027
*/

//------------------------------------------Output----------------------------------------------------------
/*
    216.58.196.164
*/