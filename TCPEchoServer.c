#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/types.h> 


#define MAX 255 
// #define PORT 8080 
#define SA struct sockaddr 

// Function designed for chat between client and server. 
void echo(int sockfd, struct sockaddr_in cliAddr) 
{ 
	char buff[MAX]; 
	int n; 
	const char * clientAddress = inet_ntoa(cliAddr.sin_addr);
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From %s: %s\n",clientAddress , buff); 

		// and send that buffer to client 
		
		if(write(sockfd, buff, sizeof(buff))) printf("Echo back to %s : %s\n", clientAddress, buff); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 

// Driver function 
int main(int argc, char *argv[]) 
{ 
	int sockfd, connfd, len; /* Socket */
	struct sockaddr_in servaddr; /* Local address */
    struct sockaddr_in cliAddr; /* Client address */
    unsigned int cliAddrLen;         /* Length of incoming message */
    unsigned short PORT;     /* Server port */
    int recvMsgSize;                 /* Size of received message */

    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    PORT = atoi(argv[1]);  /* First arg:  local port */

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cliAddr); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cliAddr, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// echo fucntion between client and server 
	echo(connfd, cliAddr); 

	// After chatting close the socket 
	close(sockfd); 
} 
