/* main.c -- main module
 *
 * 	Had lots of interesting ideas for UDP-based file service, maybe local
 * 	multi-cast, but ultimately just didn't have the time I'd like to have. 
 * 	C is not a language that lends it self easily to quickly expressing 
 * 	such grand ideas.
 *
 * 	The following is very simple and not very thorough, but should give one 
 * 	an idea of basic competence.
 *
 *			Ryan McDougall -- 2008
 *
 * Using excerpts from (especially signal safety):
 * 	Unix Systems Programming: Communication, Concurrency, and Threads
 * 	by Kay A. Robbins, Steven Robbins
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE (2*4096)

//=============================================================================
// structures

struct fileobj
{
    void *buf;
    size_t block;
    size_t size;
    int fd;
};

struct serviceobj
{
    int port;
    int socket;
};

struct servicerequestobj
{
    pthread_t tid;

    struct fileobj *file;
    int socket;
};

//=============================================================================
// forward declarations

void print_usage_and_exit (char **argv);
char get_single_option (char *opt);
void run_server (int port, const char *filename);
void run_client (const char *serveraddr, int port);
void* handle_service_request (void *arg);

int open_service (struct serviceobj *s);
int accept_service (struct serviceobj *s);
int request_service (struct serviceobj *s, const char *addr);
void close_service (struct serviceobj *s);

int load_file (const char *name, struct fileobj *f);
void unload_file (struct fileobj *f);
int send_file (struct fileobj *f, int clientsocket);

int ignore_sigpipe ();
struct sockaddr* init_sockaddr (const char *addr, int port, struct sockaddr_in *sin);
struct sockaddr* init_sockaddr_port (int port, struct sockaddr_in *sin);

//=============================================================================
// Main entry point

int main (int argc, char** argv)
{
    char mode;
    const char *file;
    const char *addr;
    int port;

    if (argc != 4)
	print_usage_and_exit (argv);

    if ((mode = get_single_option (argv [1])) == 's')
    {
	port = atoi (argv [2]);
	file = argv [3];
	run_server (port, file);
    } 
    else if ((mode = get_single_option (argv [1])) == 'c')
    {
	addr = argv [2];
	port = atoi (argv [3]);
	run_client (addr, port);
    }
    else
	print_usage_and_exit (argv);

    return 0;
}

//=============================================================================
// client / server implementation

void run_server (int port, const char *filename)
{
    struct fileobj file;
    struct serviceobj service;

    if (load_file (filename, &file) <  0)
	exit (EXIT_FAILURE);

    service.port = port;
    if ((open_service (&service)) < 0)
    {
	unload_file (&file);
	exit (EXIT_FAILURE);
    }

    for (;;)
    {
	struct servicerequestobj *request;
	request = (struct servicerequestobj *) malloc (sizeof (struct servicerequestobj));

	request->file = &file;
	if ((request->socket = accept_service (&service)) < 0)
	    perror ("giving up service");
	else
	{
	    if (pthread_create (&(request->tid), NULL, handle_service_request, request) != 0)
		perror ("unable to create thread");
	}
    }

    unload_file (&file);
    close_service (&service);
}

void* handle_service_request (void *arg)
{
    int bytessent;
    struct servicerequestobj *req = (struct servicerequestobj*) arg;
    
    bytessent = send_file (req->file, req->socket);
    printf ("%d bytes sent\n", bytessent);

    free (req);

    return NULL;
}

void run_client (const char *serveraddr, int port)
{
    struct serviceobj service;
    char buf [BUF_SIZE];
    int bytesrecv;

    service.port = port;
    if (request_service (&service, serveraddr) < 0)
	perror ("unable to connect");
    else
    {
	while (((bytesrecv = read (service.socket, buf, BUF_SIZE)) == -1) && (errno == EINTR));
	
	printf ("%d bytes recieved\n", bytesrecv);
    }

    close_service (&service);
}

//=============================================================================
// socket operations

int open_service (struct serviceobj *s)
{
    static const size_t MAXBACKLOG = 10;

    struct sockaddr_in server;
    struct sockaddr *sptr;
    int yes = 1;

    if ((sptr = init_sockaddr_port (s->port, &server)) == NULL)
    {
	perror ("unable to init socket address");
	return -1;
    }

    if (ignore_sigpipe () == -1)
    {
	perror ("unable to ignore SIG_PIPE");
	return -1;
    }

    if ((s->socket = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
	perror ("unable to open socket");
	return -1;
    }

#ifdef DEBUG
    if (setsockopt (s->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int)) == -1) 
    {
	perror ("unable to set socket options");
	return -1;
    }
#endif

    if ((bind (s->socket, sptr, sizeof (server)) < 0) || 
	    (listen (s->socket, MAXBACKLOG) < 0))
    {
	perror ("unable to bind or listen to port");
	return -1;
    }

    return 0;
}

int accept_service (struct serviceobj *s)
{
   int ret;
   struct sockaddr_in client;
 
   size_t clen = sizeof (struct sockaddr);
   struct sockaddr *cptr = (struct sockaddr*) &client;

   while (((ret = accept (s->socket, cptr, &clen)) == -1) 
	   && (errno == EINTR));

   if (ret == -1)
       perror ("unable to accept connection");
   else
       printf ("server: got connection from %s\n", inet_ntoa (client.sin_addr));
   
   return ret;
}

int request_service (struct serviceobj *s, const char *addr)
{
    struct sockaddr_in server;
    struct sockaddr *sptr;
    fd_set socketset;
    int error, ret;

    if ((sptr = init_sockaddr (addr, s->port, &server)) == NULL)
    {
	perror ("unable to init socket address");
	return -1;
    }

    if (ignore_sigpipe () == -1)
    {
	perror ("unable to ignore SIG_PIPE");
	return -1;
    }

    if ((s->socket = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
	perror ("unable to open socket");
	return -1;
    }

    if (((ret = connect (s->socket, sptr, sizeof (server))) == -1) &&
	    ((errno == EINTR) || (errno == EALREADY))) 
    {
	// establish conn async, instead of retry
	FD_ZERO(&socketset);
	FD_SET(s->socket, &socketset);

	while (((ret = select (s->socket+1, NULL, &socketset, NULL, NULL)) == -1) &&
		(errno == EINTR) ) 
	{
	    FD_ZERO(&socketset);
	    FD_SET(s->socket, &socketset);
	}
    }

    if (ret == -1) 
    {
	error = errno;
	while ((close (s->socket) == -1) && (errno == EINTR));
	errno = error;
	return -1;
    }

    return 0;
}

void close_service (struct serviceobj *s) { close (s->socket); }

//=============================================================================
// file operations

int load_file (const char *name, struct fileobj *f)
{
    struct stat	st;

    if ((f->fd = open (name, O_RDONLY)) < 0)
    {
	perror ("unable to open file");
	return -1;
    }

    if (fstat (f->fd, &st) < 0)
    {
	perror ("unable to stat file");
	close (f->fd);
	return -1;
    }
	
    f->size = st.st_size;
    f->block = st.st_blksize;
	    
    if ((f->buf = mmap (NULL, f->size, PROT_READ, MAP_PRIVATE, f->fd, 0)) 
	    == MAP_FAILED)
    {
	perror ("unable to mmap file");
	close (f->fd);
	return -1;
    }
		
    if (madvise (f->buf, f->size, MADV_SEQUENTIAL | MADV_WILLNEED) < 0)
	perror ("unable to madvise file");
		
    return 0;
}

void unload_file (struct fileobj *f)
{
    if (munmap (f->buf, f->size) < 0)
	perror ("unable to unmap file");

    close (f->fd);
}

int send_file (struct fileobj *f, int clientsocket)
{
    char *buf, *end;
    int total, remain, towrite, written; // in bytes

    total = 0;
    remain = f->size;
    buf = (char*) f->buf;
    end = buf + remain;

    while (buf < end)
    {
	towrite = (remain < f->block)? remain : f->block;
	printf ("trying to write %d bytes\n", towrite);

	/* handle interruption by signal */
	while (((written = write (clientsocket, buf, towrite)) == -1 ) 
		&& (errno == EINTR));

	if (written <= 0)
	{
	    perror ("unable to write to socket");
	    break;
	}

	buf += written;
	total += written;
	remain -= written;
    }

    return total;
}

//=============================================================================
// helper functions

int ignore_sigpipe () 
{
    struct sigaction act;

    if (sigaction (SIGPIPE, NULL, &act) == -1)
    {
	perror ("unable to fetch sigaction");
	return -1;
    }

    if (act.sa_handler == SIG_DFL) 
    {
	act.sa_handler = SIG_IGN;
	if (sigaction (SIGPIPE, &act, NULL) == -1)
	{
	    perror ("unable to set sigaction");
	    return -1;
	}
    }

    return 0;
}

struct sockaddr* init_sockaddr (const char *addr, int port, struct sockaddr_in *sin)
{
    sin->sin_family = AF_INET;
    sin->sin_port = htons (port);
    
    memset (sin->sin_zero, '\0', sizeof (sin->sin_zero));
    
    if (inet_aton (addr, &(sin->sin_addr)) == 0)
    {
	perror ("invalid IP address");
	sin = NULL;
    }

    return (struct sockaddr*) sin;
}

struct sockaddr* init_sockaddr_port (int port, struct sockaddr_in *sin)
{
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = INADDR_ANY;
    sin->sin_port = htons (port);

    memset (sin->sin_zero, '\0', sizeof (sin->sin_zero));

    return (struct sockaddr*) sin;
}

//=============================================================================
// option parsing

void print_usage_and_exit (char **argv)
{
    printf ("<%s> [-s <PORT> <FILENAME> | -c <IPADDR> <PORT>]\n", argv[0]);
    exit (EXIT_SUCCESS);
}

char get_single_option (char *opt)
{
    if ((strlen (opt) == 2) && (opt[0] == '-'))
	return opt [1];
    else
	return '\0';
}
