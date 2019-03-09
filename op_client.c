// calculater client code

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define OP_SIZE 1
#define NUM_SIZE 4
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	// Intialize variables
	int sock;
	struct sockaddr_in serv_adr;
	int i, result, counter;
	char packet[BUF_SIZE];

	// main parameter handling
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// Initialize socket as TCP
	sock = socket(PF_INET, SOCK_STREAM, 0);

	// Initialize and set server address information
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	// Connect with server and connection error handling
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connecting........");

	// Get the number of operand
	fputs("How many number for operation? : ", stdout);
	scanf("%d", &counter);
	packet[0] = (char)counter;

	// Get numbers following the number of operand
	for(i = 0; i < counter; i++)
	{
		printf("Number %d :", i+1);
		scanf("%d", (int*)&packet[i*NUM_SIZE + 1]);
	}

	// Remove buffer and get operator
	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &packet[counter * NUM_SIZE + 1]);

	// Send packet to server
	write(sock, packet, counter*NUM_SIZE+2);
	
	// Get result from server
	read(sock, &result, NUM_SIZE);

	// Print the result and diconnect socket
	printf("The result is %d \n", result);

	close(sock);
	return 0;
}

// Fuction for error handling
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

