/*	linux-socket-select-异步聊天室-chuaiGM-modified-2012-5-9
	talk_server.c -> char_server_chuai.cpp
	writed by hanzhongqiu 13/04/2009
	Using select() for I/O multiplexing 
*/
// common include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// network socket use
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

/* port we're listening on as default */
#define SERVERPORT 5625
/*maximum connect number*/
#define BACKLOG 10
/*maximum data buffer byte*/
#define BUFSIZE 2048

struct client_info
{
	int client_id;
	struct sockaddr_in client_address;
};

int main(int argc, char *argv[])
{
	// server listen port
	unsigned short listenPort;
	// if no port give out use default
	if(argc < 2)
	{
		listenPort = SERVERPORT;
	}
	else if(2 == argc)
	{
		int tmpint= atoi(argv[1]);
		if(tmpint<100||tmpint>65535)
		{
			printf("listenPort error = %d\n",tmpint);
			listenPort = SERVERPORT;
			printf("use default port = %d\n",SERVERPORT);
		}
		else
		{
			// in short range
			listenPort = (unsigned short)tmpint;
		}
	}
	else
	{
		// introduce usage
		printf("\nusage: %s [port]\n\n",argv[0]);
		return 0;
	}
	/*master file descriptor list*/
	fd_set master_fds;
	/*temple file descriptor list for select()*/
	fd_set read_fds;
	/*server address*/
	struct sockaddr_in server_addr;
	/*client address*/
	struct sockaddr_in client_addr;
	/*maximum file descriptor number*/
	int max_fd;
	/*listening socket file descriptor*/
	int sockfd;
	/*newly accept()ed socket file descreptor*/
	int newfd;
	/*buffer for saving client data*/
	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];
	/*number of client data*/
	int nbytes;
	/*for set socket option*/
	int opt;
	opt = SO_REUSEADDR;
    /*lenth of address*/
	int addr_len;
	/*for accept() to use*/
	int size;
	size = sizeof(struct sockaddr);
	socklen_t sockLenSize = socklen_t(size);
	/*temple varient*/
	int tmp_i, tmp_j;
	struct client_info clientinfo[BACKLOG];
	
	/*clear the master and temple file descriptor*/
	FD_ZERO(&master_fds);
	FD_ZERO(&read_fds);
	
	memset(&recv_buf, 0, BUFSIZE);
	memset(&send_buf, 0, BUFSIZE);

	/*create socket*/
	if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("!--> create socket() error:");
		exit(1);
	}
	/*set the socket*/
    if (-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
			perror("!--> setsockopt() error:");
			exit(1);
    }
	/*bind first config the socket then binding*/
	memset(&server_addr, 0, size);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(listenPort);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// bzero(&(server_addr.sin_zero), 8);
	memset(server_addr.sin_zero,'\0',sizeof(server_addr.sin_zero));
	if (-1 == bind(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)))
	{
		perror("!--> bind() socket error:");
		exit(1);
	}
	/*listen */
	if (-1 == listen(sockfd, BACKLOG))
	{
		perror("!--> listen() error:");
		exit(1);
	}
	/*add sockfd to master set*/
	FD_SET(sockfd, &master_fds);
	/*keep track the max file descriptor*/
	max_fd = sockfd;
	
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
	printf("O-----------------------  Server  -------------------------O\n");
	printf("> server is ok!\n");
	printf("> server listen port = %d\n",listenPort);
	/*loop*/
	while (1)
	{
		read_fds = master_fds;
		if (-1== select(max_fd + 1, &read_fds, NULL, NULL, NULL))
		{
			perror("!--> select() error!\n");
			exit(1);
		}
		/*looking for data to read*/
		for (tmp_i = sockfd; tmp_i <= max_fd; tmp_i++)
		{
			/*got connect*/
			if (FD_ISSET(tmp_i, &read_fds))
			{
				if (tmp_i == sockfd)
				{
					//newfd = accept(sockfd, (struct sockaddr*)&client_addr, &sockLenSize);
					newfd = accept(sockfd, (struct sockaddr*)&client_addr, &size);
					if (-1 == newfd)
					{
						perror("!--> accept() error:");
						exit(1);
					}
					else
					{
						clientinfo[newfd].client_id = newfd;
						clientinfo[newfd].client_address.sin_addr = client_addr.sin_addr;
						FD_SET(newfd, &master_fds);
						if (newfd > max_fd)
						{
							max_fd = newfd;
						}
						printf("> Get the new connect from [%15s]\n", inet_ntoa(client_addr.sin_addr));
					}
				}
				else
				{/*get data from the client*/
					nbytes = read(tmp_i, recv_buf, sizeof(recv_buf));
					if (0 > nbytes)
					{
						perror("!--> recv() error:");
						exit(1);
					}
					else if(nbytes == 0 || (0 == strcmp(recv_buf, "exit")))
					{	
						printf("> client: [%15s] exit!\n", inet_ntoa(clientinfo[tmp_i].client_address.sin_addr));
						FD_CLR(tmp_i, &master_fds);
						close(tmp_i);
						strcat(send_buf, inet_ntoa(clientinfo[tmp_i].client_address.sin_addr));
						strcat(send_buf, "  was exit!");
						for (tmp_j = sockfd + 1; tmp_j <= max_fd; tmp_j++)
						{
							if (FD_ISSET(tmp_j, &master_fds))
							{

								if (-1 == write(tmp_j, send_buf, nbytes))
								{
									perror("!--> send data error:");
								}
							}
						}// end for
					}
					else
					{
						printf(" get data: %5d (bytes) from the client: ", strlen(recv_buf));
						printf("[%15s]\n", inet_ntoa(clientinfo[tmp_i].client_address.sin_addr));
						strcat(send_buf, inet_ntoa(clientinfo[tmp_i].client_address.sin_addr));
						strcat(send_buf, " said: ");
						strcat(send_buf, recv_buf);
						for (tmp_j = sockfd + 1; tmp_j <= max_fd; tmp_j++)
						{
							if (FD_ISSET(tmp_j, &master_fds))
							{

								if (-1 == write(tmp_j, send_buf, nbytes))
								{
									perror("send data error:");
								}
							}
						}// end for
					}//end else
				}//end else
			}//end if
		}//end for 

    memset(&recv_buf, 0, BUFSIZE);
	memset(&send_buf, 0, BUFSIZE);
	//FD_ZERO(&master_fds);
	//FD_SET(sockfd, &master_fds);
	}//end while

	printf("\n");
	printf("O-----------------------  Server  -------------------------O\n");
	printf("                  Quit chuai' chat room \n");
	printf("O----------------------------------------------------------O\n");

return 0;
}

