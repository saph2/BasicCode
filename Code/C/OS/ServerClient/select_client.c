#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORT		5555
#define MESSAGE		"Are we having fun yet?"
#define SERVERHOST 	"127.0.0.1"

void init_sockaddr(struct sockaddr_in *name,
					const char *hostname, unsigned short int port)
{
	struct hostent *hostinfo;

	name->sin_family = AF_INET;
	name->sin_port = htons (port);
	hostinfo = gethostbyname (hostname);
	if (hostinfo == NULL) 
	{
		fprintf (stderr, "Unknown host %s.\n", hostname);
		exit (EXIT_FAILURE);
	}
	name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}

void write_to_server(int filedes)
{
	int nbytes;

	nbytes = write (filedes, MESSAGE, strlen (MESSAGE) + 1);
	if (nbytes < 0)
	{
		perror ("write");
		exit (EXIT_FAILURE);
	}
}


int main(void)
{
	int sock;
	struct sockaddr_in servername;

	/* Create the socket.	 */
	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror ("socket (client)");
		exit (EXIT_FAILURE);
	}

	/* Connect to the server.	 */
	init_sockaddr (&servername, SERVERHOST, PORT);
	if (0 > connect (sock, (struct sockaddr *) &servername, sizeof (servername)))
	{
		perror ("connect (client)");
		exit (EXIT_FAILURE);
	}

	/* Send data to the server.	 */
	write_to_server (sock);

	/* Finally exit */
	close (sock);
	exit (EXIT_SUCCESS);
}
