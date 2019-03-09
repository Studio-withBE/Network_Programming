// calculater server code

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define OP_SIZE 1
#define NUm_SIZE 4
#define BUF_SIZE 1024

void error_handling(char *message);
int calculation(int num, int packet[], char op);

int main(int argc, char *argv[])
{
	// initialize variables
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_size;
	int i, result, tmp, recv_len, recv_cnt;
	char packet[BUF_SIZE];

	// main parameter handling
	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// initialize server socket as IPv4 TCP
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error!");

	// initialize server address information
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error!");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	clnt_size = sizeof(clnt_adr);

	for(i = 0; i < 5; i++)
	{
		tmp = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_size);
		read(clnt_sock, &tmp, 1);

		recv_len = 0;

		while((tmp * OP_SIZE + 1) > recv_len)
		{
			recv_cnt = read(clnt_sock, &packet[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}

		result = calculation(tmp, (int*) packet, packet[recv_len - 1]);
		write(clnt_sock, (char *)&result, sizeof(result));
		close(clnt_sock);
	}

	return 0;
}

// Fuction for error handling of socket functions
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int calculation(int num, int packet[], char op)
{
	int result = packet[0], i;
	
	switch(op)
	{
		case '+':
			for(i = 1; i < num; i++) result += packet[i];
			break;
		
		case '-':
			for(i = 1; i < num; i++) result -= packet[i];
			break;

		case '*':
			for(i = 1; i < num; i++) result *= packet[i];
			break;
	}

	return result;
}
