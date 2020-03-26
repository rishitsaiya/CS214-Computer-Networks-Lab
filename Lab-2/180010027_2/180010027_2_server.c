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

int main() { 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; //serverAddr in git
	struct sockaddr_in newAddr;
    char str[200]; 
    int addrlen = sizeof(address); 
    char buffer[1024] = { 0 }; 
    char* hello = "Hello from server";
  	pid_t childpid;
	socklen_t addr_size;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    }

	memset(&address, '\0', sizeof(address));
  
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT); 
  
    // Forcefully attaching socket to the port 8090 
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    // puts the server socket in passive mode 
    if (listen(server_fd, 10) < 0) { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    char s[100];
  	
	while(1){
		
		if ((new_socket = accept(server_fd, (struct sockaddr*)&newAddr, &addr_size)) < 0) { 
			perror("accept"); 
			exit(EXIT_FAILURE); 
    	}

		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		
		if((childpid = fork()) == 0){
			close(server_fd);
		
			while(1){
				// read string send by client 
				bzero(str, 200);
				recv(new_socket, str, 200, 0);

				int i, j, temp; 
			
				printf("\nString sent by client: %s\n", str); 
				// printf("Sent to client %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

				int count_words = 0, count_sentences = 0;
				// scanf("%[^\n]s", str);
				
				if(strcmp(str,"exit\n") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}

				for (i = 0; str[i] != '\0'; i++) {
					if (str[i] == ' ' && str[i+1] != ' ')
						count_words++;
					if(str[i] == '.')
						count_sentences++;
				}
				
				char s1[5], s2[5], s3[5];
				int size = strlen(str) - 1;
				itoa(size, s1, 10);
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
				printf("%s\n", s);
				sleep(strlen(s));
				send(new_socket, s, sizeof(s), 0);
				bzero(s, 100); 

				printf("Sent to client %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			}
		}
	}

	close(new_socket);
    return 0; 
}

//------------------------------------------Directions----------------------------------------------------------
/*
    CLI:
        
        gcc 180010027_2_server.c -o server
        ./server
*/

//------------------------------------------Output------------------------------------------------------------------

/*
	Connection accepted from 254.127.0.0:36102
	Connection accepted from 127.0.0.1:49314

	String sent by client: this is test. hello world.

	26,5,2
	Sent to client 127.0.0.1:49314

	String sent by client: this is hello. hello world.

	27,5,2
	Sent to client 254.127.0.0:36102

*/