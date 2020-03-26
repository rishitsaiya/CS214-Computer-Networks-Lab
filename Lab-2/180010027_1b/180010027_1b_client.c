#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 

#define PORT 8090 

// Driver code 
int main() 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char str[100]; 
  
    char buffer[1024] = { 0 }; 
  
    // Creating socket file descriptor 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
  
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
  
// Convert IPv4 and IPv6 addresses from text to binary form 127.0.0.1 is local 
// host IP address, this address should be your system local host IP address 
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){ 
        printf("\nAddress not supported \n"); 
        return -1; 
    } 
  
    // connect the socket 
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("\nConnection Failed \n"); 
        return -1;
    } 
 
    char s[100];
    printf("\nInput the string: ");
  while(1){
  	
    // scanf("%[^\n]s", str); 
	bzero(str, 100);
    fgets(str, 100, stdin);
    // int l = strlen(str); 
  		// if(str == "-q"){
  		// 	exit(1);
  		// }
    // send string to server side 
    send(sock, str, sizeof(str), 0);

    if(strcmp(str,"exit\n") == 0){
		exit(1);
	}
  
    // read string sent by server 
    bzero(str, 100);
    valread = read(sock, str, 100); 
    // printf("%s\n", str);

	int count = 0,n;
	printf("\nNumber of characters: ");
	for(int i = 0; i < strlen(str); i++){
		if(str[i] >= '0' && str[i] <= '9'){
            n = str[i] - '0';
			printf("%d", n);
		}
		else{
			count++;
			printf("\n");
			if (count == 1)
				printf("Number of words: ");
			else if(count == 2){
				printf("Number of sentences: ");
			}
		}
	}
	printf("\n\nInput the string: ");
    // bzero(str, 100);
}
    return 0; 
}

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_1b_client.c -o client
        ./client
*/

//------------------------------------------Output----------------------------------------------------------
/*
    Client1:

    Input the string: hi. this is hello world.          

    Number of characters: 24
    Number of words: 5
    Number of sentences: 2

    Client2:

    Input the string: this is test. hi bro,

    Number of characters: 21
    Number of words: 5
    Number of sentences: 1
*/