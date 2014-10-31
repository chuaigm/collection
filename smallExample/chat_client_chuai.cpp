/*	linux-socket-select-异步聊天室-chuaiGM-modified-2012-5-9
	char_client_chuai.cpp
	writed by hanzhongqiu 13/04/2009
	Using select() for I/O multiplexing 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERVERPORT 5625
#define BUFSIZE 2048

int main(int argc, char *argv[])
{
	// ip and nickname
	char serverIPV4[16];
	memset(serverIPV4,0,24);
	unsigned short serverPort = 0;
	char nickname[24];
	memset(nickname,0,24);

	switch(argc)
	{
	// only one argment
	case 2:
		break;
	// two argment
	case 3:
		memcpy(nickname,argv[2],sizeof(argv[2]));
		break;
	default:
		{
			// hint!
			printf("!--> Please input the server ip:port! like:\n");
			printf(" %s 127.0.0.1:5625\n",argv[0]);
			printf(" %s [serverIP] [nickname]\n",argv[0]);
			return 0;	
		}
	}
	// parser ip and port string
	for(int i=0; i<strlen(argv[1]);i++)
	{
		if(':'==argv[1][i])
		{
			// get ip
			memcpy(serverIPV4,argv[1],i);
			serverIPV4[i]='\0';
			// get port
			char tmp[8];
			memset(tmp,0,sizeof(tmp));
			memcpy(tmp,&argv[1][i+1],strlen(argv[1])-i);
			int tmpi = atoi(tmp);
			if(tmpi<100||tmpi>65535)
			{
				printf("input port error = %d\n",tmpi);
				serverPort = SERVERPORT;
				printf("use default port = %d\n",SERVERPORT);
			}
			else
			{
				serverPort = (unsigned short)tmpi;
			}
			break;
		}
		// if no ':' to give port
		if( i+1 == strlen(argv[1]))
		{
			memcpy(serverIPV4,argv[1],strlen(argv[1]));
			serverPort = SERVERPORT;
		}
	}
	printf(" connect server ip = %s\n",serverIPV4);
	printf(" connect server port = %d\n",serverPort);
	// socket
	int sockfd;
	fd_set sockset;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int recvbytes;
	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];
	int data_len;
	// create socket
	if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0 )))
	{
		perror("!--> create socket errors are :");
		exit(1);
	}
	memset(&serveraddr, 0, sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(serverIPV4);
	serveraddr.sin_port = htons(serverPort);

	if (-1== connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr)))
	{
		perror("!--> connect error:");
		exit(1);
	}
    FD_ZERO(&sockset); 
	FD_SET(sockfd, &sockset); 
	FD_SET(0, &sockset); 

	/* get local host IP */
	char hname[128];
	struct hostent *hent;
	gethostname(hname, sizeof(hname));
	hent = gethostbyname(hname);

	printf("hostname: %s\naddress list:\n", hent->h_name);
	for(int i = 0; hent->h_addr_list[i]; i++) {
		printf("\t[%15s]\n", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
		}

	printf("V----------------------------------------------------------V\n");
	printf("                Welcome to chuai' chat room \n");
	printf("O-----------------------  Client  -------------------------O\n");
	printf("> client ok!\n");

    while (1)
    {
		memset(recv_buf, 0, sizeof(recv_buf));
		memset(send_buf, 0, sizeof(send_buf));
		select( sockfd+1, &sockset, NULL, NULL, NULL); 
		if (FD_ISSET( sockfd, &sockset)) 
		{
			if (-1 == (recvbytes =read(sockfd, recv_buf, sizeof(recv_buf))))
			{
				perror("!--> read data error:");
				close(sockfd);
				exit(1);
			}
			else if (recvbytes == 0)
			{
				perror("!--> Server is down ...");
				close(sockfd);
				exit(1);
			}
			else
			{
				recv_buf[recvbytes] = '\0';
				printf("\n");
				printf("V----------------------------------------------------------V\n");
				printf("%s\n", recv_buf);
				printf("O----------------------------------------------------------O\n");
				printf("\n");
				fflush(stdout);
			}
		}
		if ( FD_ISSET( 0, &sockset)) 
		{
			fgets(send_buf, sizeof(send_buf), stdin);
			data_len = strlen(send_buf);
			send_buf[data_len - 1] = '\0';
			//send the content
			if (-1 == write(sockfd, send_buf, sizeof(send_buf)))
			{
				perror("!--> send data error:");
				close(sockfd);
				exit(1);
			} 
			//quit the chat room
			if (0 == strcmp(send_buf, "exit"))
			{
				printf("> quiting from chat room!\n");
				close(sockfd);
				exit(1);
			}
		}
			//重新设置 sockset. (即将 sockset 清空, 并将 sockfd 和 0 加入) 
			FD_ZERO(&sockset);
			FD_SET(sockfd, &sockset);
			FD_SET(0, &sockset);
	}//end while
	close(sockfd);

	printf("\n");
	printf("O-----------------------  Client  -------------------------O\n");
	printf("                  Quit chuai' chat room \n");
	printf("O----------------------------------------------------------O\n");

	return 0;
}

