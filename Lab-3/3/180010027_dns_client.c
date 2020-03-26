#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

// #define PORT 50027
#define MAXLINE 1024 

// Driver code 
int main(int argc, char** argv) {

	if(argc < 3){
        printf("Enough arguments not provided.\n");
        exit (0);
    }
	
	int PORT = atoi (argv[2]);
	int sockfd; 
	char buff[MAXLINE]; 
	// char *hello = "Hello from client"; 
	struct sockaddr_in servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr(argv[1]); 
	
	int n, len;
	printf("DNS name to be entered here: ");
	fgets(buff, MAXLINE, stdin);
	sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	
	while (1){
	
	// printf("Hello message sent.\n"); 

		bzero(buff, MAXLINE);
		n = recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
		// if (strcmp(buff, "exit\n") == 0){
		// 	exit(1);
		// }
		buff[n] = '\0'; 
		// printf("Server : %s\n", buff);
		if((strcmp(buff, "done")==0)){
			printf("Done\n");
			exit(1);
		}

		else if((strcmp(buff, "Error!!")==0)){
            exit(0);
        }
		else{
			printf("IP Address : %s\n", buff);
		}
	}
		
	close(sockfd); 
	
	return 0; 
}

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_dns_client.c -o client
        ./client 0.0.0.0 50027
*/

//------------------------------------------Output----------------------------------------------------------
/*
    DNS name to be entered here: www.google.com
	IP Address : 216.58.196.164
	Done

*/