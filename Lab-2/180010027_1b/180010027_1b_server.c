#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
  
#define PORT 8090

char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
  
// Driver code 
int main() { 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    char str[100]; 
    int addrlen = sizeof(address); 
    char buffer[1024] = { 0 }; 
    char* hello = "Hello from server"; 
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 
  
    // Forcefully attaching socket to 
    // the port 8090 
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    // puts the server socket in passive mode 
    if (listen(server_fd, 3) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
  
    char s[100];
  while(1){
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

    // read string send by client 
    bzero(str, 100);
    valread = read(new_socket, str, sizeof(str)); 
    int i, j, temp; 
  
    printf("\nString sent by client: %s\n", str); 
    int count_words = 0, count_sentences = 0;
	// scanf("%[^\n]s", str);

    if(strcmp(str,"exit\n") == 0){
		printf("Disconnected from client.\n");
		break;
	}
    
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ' && str[i+1] != ' ')
            count_words++;
        if(str[i] == '.')
        	count_sentences++;
    }
    
    char s1[5], s2[5], s3[5];

    itoa(strlen(str)-1, s1, 10);
    itoa(count_words+1, s2, 10);
    itoa(count_sentences, s3, 10);


    for (int i = 0; i < strlen(s1); i++){
		s[i] = s1[i];
	}

	s[strlen(s1)] = ',';

	for (int i = strlen(s1) + 1; i < strlen(s1) + strlen(s2) + 1; i++){
		s[i] = s2[i - strlen(s1) - 1];
	}

	s[strlen(s1) + strlen(s2) + 1] = ',';

	for (int i = strlen(s1) + strlen(s2) + 2; i < strlen(s1) + strlen(s2) + strlen(s3) + 2; i++){
		s[i] = s3[i - strlen(s1) - strlen(s2) - 2];
	}
	// printf("%s\n", s);
    send(new_socket, s, sizeof(s), 0);
    bzero(s, 100); 

    printf("Sent to client.\n");
	} 
  
    return 0; 
}

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_1b_server.c -o server
        ./server
*/

//------------------------------------------Output----------------------------------------------------------
/*
    String sent by client: hi. this is hello world.

    Sent to client.

    String sent by client: this is test. hi bro,

    Sent to client.
*/