#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/mman.h>


/************* globals **************/

/*mutex and shared variable*/
pthread_t *threads;
pthread_mutex_t global_lock; /*mutex for access to shared "data"*/
pthread_cond_t notEmpty;
int done = 0; /* used to signal consumers they should exit gracefuly*/
int isEmpty = 1;

/*queue variable*/
static int queue_size = 0;
static int max_requests = 0;
static int thread_num = 0;
/************************************/

/*functions*/
#define ISspace(x) isspace((int)(x))

/*************************************** STRUCT *************************************/
struct Socket_Instance{
	int socket_client;
	struct Socket_Instance *next;
}typedef socket_a;

struct Socket_Queue{
	struct Socket_Instance *head;
	struct Socket_Instance *tail;
}typedef socket_q;

struct Socket_Queue *socket_queue;/*global queue*/

/*********************************************** SIG AND FREE HANDLE *****************************************/

void free_Queue(struct Socket_Queue *socket_queue){
	/*free socket queue*/
	struct Socket_Instance *socket_ins;
	if (socket_queue->head==NULL){
		return;
	}
	while ((socket_ins = socket_queue->head)!= socket_queue->tail){/*go until one before tail*/
		socket_queue->head = socket_queue->head->next;
		close(socket_ins->socket_client);
		free(socket_ins);
	}
	close(socket_ins->socket_client);
	free(socket_ins);
}

void close_all(){
	/*close all open memory*/
	sleep(2);
	int i, rc;
	if (socket_queue != NULL){
		free_Queue(socket_queue);
		free(socket_queue);
	}
	if (threads != NULL){
		done = 1;
		for (i = 0; i < thread_num; i++) {/*close all threads*/
			rc = pthread_mutex_lock(&global_lock); assert(!rc);
			pthread_cond_broadcast(&notEmpty);
			rc = pthread_mutex_unlock(&global_lock); assert(!rc);
		}
		pthread_mutex_destroy(&global_lock); /* destroy shared lock */
		pthread_cond_destroy(&notEmpty);
		free(threads);
	}
	return;
}

void signal_handler(int signum){
	/*signal handler*/
	close_all();
	exit(0);
}

/********************************** HTTP REQUESTS ************************************/

void send_to_client(int client, char *msg, int flag){
	send(client, msg, strlen(msg), flag);
}

void ok_http_head(int client){
	/*print the begining of a valid request*/
	send_to_client(client, "HTTP/1.1 200 OK\r\n", 0);
	send_to_client(client, "Content-Type: text/html\r\n", 0);
	send_to_client(client,"\r\n", 0);
	send_to_client(client,"<HTML><HEAD><TITLE>GET method\r\n", 0);
	send_to_client(client,"</TITLE></HEAD>\r\n", 0);
	send_to_client(client,"\r\n", 0);
}

void not_implemented(int client){
	/*return Not Implemented 501 to client*/
	send_to_client(client, "HTTP/1.0 501 Method Not Implemented\r\n", 0);
	send_to_client(client, "Content-Type: text/html\r\n", 0);
	send_to_client(client, "\r\n", 0);
	send_to_client(client, "<HTML><HEAD><TITLE>Method Not Implemented\r\n", 0);
	send_to_client(client, "</TITLE></HEAD>\r\n", 0);
	send_to_client(client, "<BODY><P>HTTP request method not supported.\r\n", 0);
	send_to_client(client, "</BODY></HTML>\r\n", 0);
}

void not_found(int client){
	/*return Not Found 404 to client*/
	send_to_client(client, "HTTP/1.0 404 NOT FOUND\r\n", 0);
	send_to_client(client, "Content-Type: text/html\r\n", 0);
	send_to_client(client, "\r\n", 0);
	send_to_client(client, "<HTML><TITLE>Not Found</TITLE>\r\n", 0);
	send_to_client(client, "<BODY><P>The server could not fulfill\r\n", 0);
	send_to_client(client, "your request because the resource specified\r\n", 0);
	send_to_client(client, "is unavailable or nonexistent.\r\n", 0);
	send_to_client(client, "</BODY></HTML>\r\n", 0);
}

void service_unavailable(int client){
	/*return Service Uavailable to client*/
	send_to_client(client, "HTTP/1.0 503 Service Unavailable\r\n", 0);
	send_to_client(client, "Content-Type: text/html\r\n", 0);
	send_to_client(client, "\r\n", 0);
	send_to_client(client, "<HTML><TITLE>Service Unavailable</TITLE>\r\n", 0);
	send_to_client(client, "<BODY><P>The server could not fulfill\r\n", 0);
	send_to_client(client, "your request because the resource specified\r\n", 0);
	send_to_client(client, "is unavailable or nonexistent.\r\n", 0);
	send_to_client(client, "</BODY></HTML>\r\n", 0);
}

void dir_contant(int client, DIR *dir){
	/*print directory contant to http*/
	struct dirent *ent;
	char *file;
	ok_http_head(client);
	send_to_client(client, "</BODY><P>\r\n", 0);
	while ((ent = readdir(dir)) != NULL) { /* print all the files and directories within directory */
		file = ent->d_name;
		send_to_client(client, file, 0);
		send_to_client(client, "\r\n", 0);
	}
	send_to_client(client, "</BODY></HTML>\r\n", 0);
	closedir(dir);
}

void file_contant(int client, int fd){
	/*print file contant to http*/
	char *buf_file;
	char *line;
	int offset = 0;
	int i = 0;
	int size = lseek(fd, -1, SEEK_END);
	if (size < 0){
		close(fd);
		not_found(client);
		return;
	}
	buf_file = (char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);/*mmapp source file*/
	if (buf_file == MAP_FAILED){
		close(fd);
		close(client);
		close_all();
		printf("Error mmapping the file\n");
		exit(1);
	}
	ok_http_head(client);
	send_to_client(client, "</BODY><P>\r\n", 0);
	send_to_client(client, buf_file, 0);
	send_to_client(client, "\r\n", 0);
	send_to_client(client, "</BODY></HTML>\r\n", 0);
	close(fd);
}

int get_line(int client, char *buf, int size){
	/*read line from socket - char by char*/
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(client, &c, 1, 0);
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(client, &c, 1, MSG_PEEK);
				if ((n > 0) && (c == '\n'))
					recv(client, &c, 1, 0);
				else
					c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';

	return(i);
}

void handel_request(int client){
	/*handel request that has been dequeded*/
	char buf[1024];
	int numchars;
	char method[255];
	char url[255];
	char path[512];
	size_t i, j;
	int fd;
	DIR *dir;
	char *query_string = NULL;
	numchars = get_line(client, buf, sizeof(buf));
	i = 0; j = 0;
	while (!ISspace(buf[j]) && (i < sizeof(method)-1))
	{
		method[i] = buf[j];
		i++; j++;
	}
	method[i] = '\0';

	if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{
		not_implemented(client);/*not implemented*/
		return;
	}
	i = 0;
	while (ISspace(buf[j]) && (j < sizeof(buf))){
		j++;
	}
	while (!ISspace(buf[j]) && (i < sizeof(url)-1) && (j < sizeof(buf)))
	{
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';
	while ((numchars > 0) && strcmp("\n", buf)){  /* read & discard headers */
		numchars = get_line(client, buf, sizeof(buf));
	}
	if ((dir = opendir(url)) != NULL){/*dir*/
		dir_contant(client, dir);
	}
	else if ((fd = open(url, O_RDWR, S_IRWXU)) >= 0){/*file*/
		file_contant(client, fd);
	}
	else{
		not_found(client);/*not found*/
	}
}

/************************************************ ENGQUEQE & DEQUEUE ********************************************/

int enqueue(int client) {
	/*enqueue item */
	int rc;
	if (queue_size == max_requests){
		return -1;
	}
	struct Socket_Instance *socket_ins=(struct Socket_Instance*) malloc(sizeof(struct Socket_Instance));	
	if (socket_ins==NULL){
	 exit(1);
	}
	socket_ins->next=NULL;
	rc = pthread_mutex_lock(&global_lock); /* thread gets lock */	
	if (rc != 0) {
		printf("Thread lock failed\n");
		exit(1);
	}
	if (socket_queue->tail == NULL){
		socket_queue->tail=socket_ins;
		socket_queue->head=socket_queue->tail;		
	}
	else{
		socket_queue->tail->next=socket_ins;
		socket_queue->tail=socket_queue->tail->next;
	}
	socket_queue->tail->socket_client = client;
	if (rc != 0){
		exit(1);
	}
	queue_size++;
	isEmpty = 0;
	rc = pthread_cond_signal(&notEmpty);/* wake up a thread */	
	if (rc != 0) {
		printf("Thread condsignal failed\n");
  	    exit(1);
	 }
	rc = pthread_mutex_unlock(&global_lock);/*finshed increasing - release lock*/
	if (rc != 0) {
		printf("Thread unlock failed\n");
		exit(1);
	}
	return 1;
}

void *dequeue(void *args) {
	/*thread work - dequeue item */
	int rc;
	int client;
	struct Socket_Instance *socket_ins;
	rc = pthread_mutex_lock(&global_lock); /* thread gets lock */
	if (rc != 0){
		printf("Thread lock failed\n");
		exit(1);
	}
	while (!done){
		while (isEmpty){
			rc = pthread_cond_wait(&notEmpty, &global_lock);
			if (rc != 0) {
				printf("Thread condwait failed\n");
				pthread_mutex_unlock(&global_lock);
				exit(1);
			}
			if (done){
				break;
			}
		}
		if (done){
			rc=pthread_mutex_unlock(&global_lock);
			if (rc != 0){
				printf("Thread unlock failed\n");
				exit(1);
			}
			return NULL;
		}
		if (socket_queue->head == NULL){
			rc=pthread_mutex_unlock(&global_lock);
			if (rc != 0){
				printf("Thread unlock failed\n");
				exit(1);
			}
			return NULL;
		}
		else{/*dequeue from head of queue*/
			socket_ins = socket_queue->head;
			client = socket_ins->socket_client;
			if (socket_queue->head->next==NULL){
				socket_queue->tail=NULL;
				socket_queue->head=socket_queue->tail;
			}
			else{
				socket_queue->head=socket_queue->head->next;
			}
			socket_ins->next = NULL;
			queue_size--;
		}
		if (queue_size == 0){
			isEmpty = 1;
		}
		rc = pthread_mutex_unlock(&global_lock);/*finshed increasing - release lock*/
		if (rc != 0) {
			printf("Thread unlock failed\n");
			exit(1);
		}
		free(socket_ins);
		handel_request(client);
		close(client);
		sleep(1);
	}
	return NULL;
}

int init_prog_and_threads(pthread_t *threads){
	/*init the program memory and threads*/
	int rc;
	socket_queue = (struct Socket_Queue*) malloc(sizeof(struct Socket_Queue));/*create Socket_Queue*/
	socket_queue->tail=NULL;
	socket_queue->head=socket_queue->tail;
	if (socket_queue == NULL){
		perror("Error - error creating Squeue!\n");
		return -1;
	}
	rc = pthread_mutex_init(&global_lock, NULL);/*init lock*/
	if (rc != 0){
		free(socket_queue);
		free(threads);
		perror("Error - error creating lock!\n");
		return -1;
	}
	rc = pthread_cond_init(&notEmpty, NULL);/*init condition*/
	if (rc != 0){
		free(socket_queue);
		free(threads);
		perror("Error - error creating condition!\n");
		return -1;
	}
	int i;
	isEmpty = 1;
	for (i = 0; i < thread_num; i++){/*init threads*/
		rc = pthread_create(&threads[i], NULL, dequeue, NULL);
		if (rc != 0){
			perror("Error - error creating thread!\n");
			free(socket_queue);
			free(threads);
			return -1;
		}
	}
	return 1;
}

/***********************************************  SERVER & SOCKETS ***********************************************/

int make_and_bind_socket(unsigned short int *port){
	/*create a socket and bind it*/
	int list_socket = 0;
	struct sockaddr_in socket_addr;

	list_socket = socket(PF_INET, SOCK_STREAM, 0); /* Create the socket */
	if (list_socket < 0)
	{
		perror("list_socket");
		exit(EXIT_FAILURE);
	}
	/* Give the socket a name.	*/
	memset(&socket_addr, '0', sizeof(socket_addr));
	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(*port);
	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY = any local machine address */

	if (bind(list_socket, (struct sockaddr *)&socket_addr, sizeof (socket_addr)) < 0)
	{
		printf("\n Error : Bind Failed. %s \n", strerror(errno));
		close_all();
		exit(EXIT_FAILURE);
	}
	if (listen(list_socket, 5) < 0){
		perror("listen");
		close_all();
		exit(EXIT_FAILURE);
	}
	return list_socket;
}


/*********************************************** MAIN ************************************************/

int main(int argc, char** argv)
{
	int listenfd = -1;
	struct sockaddr_in my_addr, peer_addr;
	unsigned short int port = 80;
	int addrsize = sizeof(struct sockaddr_in);
	
	assert(argc >= 3); /*First = number of threads, second = max_requests, third = port number*/
	thread_num = (int)strtol(argv[1], NULL, 10);
	max_requests = (int)strtol(argv[2], NULL, 10);
	
	if (argc == 4){
		port = (unsigned short int)strtol(argv[3], NULL, 10);
	}

	if (signal(SIGINT, signal_handler) == SIG_ERR) {
		return EXIT_FAILURE;
	}

	listenfd = make_and_bind_socket(&port);/*create the server socket for incoming requests on port*/

	threads = (pthread_t*)calloc(thread_num, sizeof(pthread_t));/*create thread_num threads*/
	
	if (threads == NULL){
		perror("Error - error creating threads!\n");
		return;
	}

	if (init_prog_and_threads(threads) < 0){/*init the program args and variables*/
		return;
	}

	printf("http server running on port %d\n", port);

	int i,rc;
	while (!done){/*enqueue requests*/
		int client = accept(listenfd, (struct sockaddr*)&peer_addr, &addrsize); /* accpeting connection & print the client socket details */
		if (client < 0){
			printf("\n Error : Accept Failed. %s \n", strerror(errno));
			exit(1);
		}
		else{
			getsockname(client, (struct sockaddr*)&my_addr, &addrsize);
			getpeername(client, (struct sockaddr*)&peer_addr, &addrsize);
			printf("connected to client! peer is %s:%d (i am %s:%d)\n", inet_ntoa((peer_addr.sin_addr)), ntohs(peer_addr.sin_port), inet_ntoa((my_addr.sin_addr)), ntohs(my_addr.sin_port));
			rc = enqueue(client);/* add request to queue*/
			if (rc<0){/*queue is full*/
			 service_unavailable(client);
			 close(client);
			 continue;
			}
		}
		sleep(10);
	}
	sleep(2);
	close(listenfd);
	close_all();
	return;
}

/*************************************************************************************************************************************************************************/



