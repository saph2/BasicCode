#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <assert.h>

#define PORT	5555
#define MAXMSG	512

int make_socket(unsigned short int port)
{
	int sock;
	struct sockaddr_in name;

	/* Create the socket.	*/
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* Give the socket a name.	*/
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *) &name, sizeof (name)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	return sock;
}

int read_from_client(int filedes)
{
	char buffer[MAXMSG];
	int nbytes;

	nbytes = read(filedes, buffer, MAXMSG);
	if (nbytes < 0)
	{
		/* Read error.	*/
		perror("read");
		exit(EXIT_FAILURE);
	}
	else if (nbytes == 0)
		/* End-of-file.	*/
		return -1;
	else
	{
		/* Data read.	*/
		fprintf(stderr, "Server: got message: `%s'\n", buffer);
		return 0;
	}
}

int main(void)
{
	int listensock;
	int status;
	fd_set active_fd_set, read_fd_set; // need duplicates...
	int i;
	struct sockaddr_in clientname;
	socklen_t size;

	/* Create the listening socket and set it up to accept connections.	*/
	listensock = make_socket(PORT);
	if (listen(listensock, 5) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* Initialize the set of active sockets.
		 and add listening socket too!	*/
	FD_ZERO(&active_fd_set);
	FD_SET(listensock, &active_fd_set);

	while (1)
	{			
		/* select() will trash read_fd_set when done. we need to do this every time!*/
		read_fd_set = active_fd_set; // copy struct
		
		/* Block until input arrives on one or more active sockets.	*/
		if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}

		/* Service all the sockets with input pending (possibly more than one...) 
			 New connection? data arriving on existing one?
			 (something is ready for reading and will not block) */
		for (i = 0; i < FD_SETSIZE; ++i)
		{
			if (FD_ISSET (i, &read_fd_set))
			{
				/* Connection request on listening socket.	*/
				if (i == listensock)
				{
					size = sizeof (clientname);

					/* we dont wait now, we definitely have someone trying to get accepted! */								
					int clientsock = accept(listensock, (struct sockaddr*)&clientname, &size);
					if (clientsock < 0)
					{
						perror("accept");
						exit(EXIT_FAILURE);
					}

					fprintf(stderr, "Server: connect from host %s, port %d.\n",
						inet_ntoa(clientname.sin_addr),
						ntohs(clientname.sin_port));
					
					FD_SET(clientsock, &active_fd_set);
				}
				/* Data arriving on an already-connected socket.	*/
				else
				{
					/* read and print client message */
					if (read_from_client (i) < 0)
					{
						close(i);
						FD_CLR(i, &active_fd_set);
					}
				}
			}
		}
	}
}
