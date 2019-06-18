/**
 * @file sockhelp.c
 * @author Philip R. Simonson
 * @date 06 Jun 2019
 * @brief Simple socket helper library (WIP).
 *
 * This is going to be a cross platform socket (networking)
 * library. Although its a work-in-progress right now. But
 * everthing that is implemented right now all works in Linux.
 * When this library is complete it will work in Windows also.
 */

#ifdef __linux	/* if linux make as _GNU_SOURCE */
#define _GNU_SOURCE 1
#endif

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Headers for socket programming */
#ifdef _WIN32
#define _WINVER 0x600
#include <windows.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include "sockhelp.h"

#define BACKLOG 100

/* ------------------------- Init Functions --------------------- */

/**
 * @brief Initialize sockets in Windows/Linux.
 */
int socket_startup()
{
#ifdef _WIN32
	WSAData wsaData;
	int res;
	
	if((res = WSAStartup(MAKEWORD(2,0), &wsaData)) != 0) {
		printf("WSAStartup() failed error code: %8X\n",
			WSAGetLastError());
	}
	return res;
#else
	return 0;
#endif
}
/**
 * @brief Shutdown sockets in windows/linux.
 */
int socket_shutdown()
{
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
#endif
}

/* ------------------------ Start Functions --------------------- */

/**
 * @brief Default main loop function for my sockhelp library.
 *
 * Returns: int
 */
static int default_loop(sock_t *sock)
{
	printf(	"default_loop: Please implement this function.\n"
		"=======================================================\n"
		"static void any_name_loop_func(sock_t *sock)\n"
		"{\n\t/* TODO: Add code here for accepting clients.\n\t *"
		" Also, handling clients. */\n}\n"
		"=======================================================\n"
		"That is the prototype for this function. Then, call:"
		"\ninit_socket(&socket, any_name_loop_func);\nto use"
		" it.\n" );
	return 0;
}
/**
 * @brief Gets address structure.
 *
 * Returns: void*
 */
void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
/**
 * @brief Init socket structure and open connection.
 *
 * Returns: void
 */
void init_socket(sock_t *sock, int (*func)(sock_t *sock))
{
	memset(sock->addr, 0, sizeof(sock->addr));
	sock->fd = INVALID_SOCKET;
	sock->errno = SOCKERR_OKAY;
	sock->loop = !func ? default_loop : func;
}
/**
 * @brief Create server socket.
 *
 * Returns: 1=failure, 0=success
 */
int server_socket(sock_t *sock, const char *port)
{
	struct addrinfo hints,*servinfo,*p;
	int yes=1,rv;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; /* use my IP */

	if((rv=getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all results */
	for(p=servinfo; p!=NULL; p=p->ai_next) {
		if((sock->fd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == INVALID_SOCKET) {
			sock->errno = SOCKERR_CREATE;
			continue;
		}

		if(setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == SOCKET_ERROR) {
			sock->errno = SOCKERR_OPTION;
			close_socket(sock);
			return 1;
		}

		if(bind(sock->fd, p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
			sock->errno = SOCKERR_BIND;
			close_socket(sock);
			continue;
		}
		break;
	}

	if(p==NULL) {
		sock->errno = SOCKERR_BIND;
		close_socket(sock);
		return 1;
	}
	inet_ntop(p->ai_addr->sa_family,
		get_in_addr((struct sockaddr*)&p->ai_addr), sock->addr,
		sizeof(sock->addr));
	freeaddrinfo(servinfo);		/* all done with this */

	if(listen(sock->fd, BACKLOG) == SOCKET_ERROR) {
		sock->fd = SOCKERR_LISTEN;
		close_socket(sock);
		return 1;
	}
	printf("server: %s listening on port %s.\n"
		"server: waiting for connections...\n",
		sock->addr, port);
	sock->errno = SOCKERR_OKAY;
	return 0;
}
/**
 * @brief Create a client connection.
 *
 * Returns: 1=failure, 0=success
 */
int client_socket(sock_t *sock, const char *addr, const char *port)
{
	struct addrinfo hints,*servinfo,*p;
	struct hostent *host;
	int rv;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if((rv=getaddrinfo(addr, port, &hints, &servinfo)) != 0) {
		return 1;
	}

	/* loop through all results */
	for(p=servinfo; p!=NULL; p=p->ai_next) {
		if((sock->fd=socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == INVALID_SOCKET) {
			sock->errno = SOCKERR_CREATE;
			continue;
		}
		if(connect(sock->fd, p->ai_addr, p->ai_addrlen)
				== SOCKET_ERROR) {
			sock->errno = SOCKERR_CONNECT;
			close_socket(sock);
			continue;
		}
		break;
	}

	if(p==NULL) {
		sock->errno = SOCKERR_CONNECT;
		close_socket(sock);
		return 1;
	}
	freeaddrinfo(servinfo);
	
	host = gethostbyname(addr);
	if(host) {
		strncpy(sock->addr, host->h_name,
			strlen(host->h_name));
		printf("client: connecting to %s\n", sock->addr);
		sock->errno = SOCKERR_OKAY;
		return 0;
	}
	printf("client: %s failed to connect.\n", addr);
	sock->errno = SOCKERR_CONNECT;
	return 1;
}
/**
 * @brief Accepts client socket from server socket.
 *
 * Returns: sock_t
 */
sock_t accept_socket(sock_t *server)
{
	struct sockaddr_storage addr;
	socklen_t sin_size = sizeof(addr);
	sock_t sock;
	
	memset(&sock, 0, sizeof(sock_t));
	sock.fd = accept(server->fd, (struct sockaddr*)&addr,
			&sin_size);
	if(sock.fd == INVALID_SOCKET) {
		sock.errno = SOCKERR_CREATE;
		return sock;
	}
	inet_ntop(addr.ss_family,
		get_in_addr((struct sockaddr*)&addr),
		sock.addr, sizeof(sock.addr));
	sock.errno = SOCKERR_OKAY;
	return sock;
}
/* Runs a main loop for given socket structure.
 * Returns: int
 */
int loop_socket(sock_t *sock, int loop_type)
{
	int running = 1;
	while(running) {
		switch(loop_type) {
		case SOCKRUN_LOOP:
			running = sock->loop(sock);
		break;
		default:
			printf("Error: unknown loop type.\n");
			running = 0;
		}
	}
	return running = (running == 0 ? 1 : 0);
}
/* Gets current error message.
 * Returns: char*
 */
const char *get_error_socket(sock_t *sock)
{
	static const char *errmsg[] = {
		"Unknown error occured.",
		"No error occured.",
		"Failed to create socket.",
		"Failed to set socket option.",
		"Failed to listen for connections.",
		"Failed to bind socket.",
		"Failed to connect to socket.",
		"Socket is closed."
	};
	return errmsg[sock->errno];
}
/* Gets current error number.
 * Returns: int
 */
int get_errori_socket(sock_t *sock)
{
	if(sock->errno < 0 || sock->errno >= SOCKERR_COUNT)
		return SOCKERR_UNKNOWN;
	return sock->errno;
}
/* Close socket and cleanup structure.
 * Returns: 1=failure,0=success
 */
int close_socket(sock_t *sock)
{
	int res = 0;
#ifdef _WIN32
	if((res = closesocket(sock->fd))) {
		printf("Error: Could not close socket.\n");
		sock->errno = SOCKERR_CLOSE;
		return res;
	}
#else
	if((res = close(sock->fd))) {
		printf("Error: Could not close socket.\n");
		sock->errno = SOCKERR_CLOSE;
		return res;
	}
#endif
	sock->fd = INVALID_SOCKET;
	sock->errno = SOCKERR_OKAY;
	return res;
}

/* ------------------------ Helper Functions --------------------- */

/* Sets whether or not it's blocking.
 * Returns: 1=failure,0=success
 */
int bmode_socket(sock_t *sock, int bmode)
{
	return 0;
}
/* Sends data across the network to another computer.
 * Returns: bytes sent (long int)
 */
long send_data(sock_t *sock, const void *data, long size, int flags)
{
	long total_bytes=0,nbytes;
	long len = size;
	char *p=(char*)data;

	while(total_bytes < len) {
		nbytes = send(sock->fd, p+total_bytes, len-total_bytes, flags);
		total_bytes += nbytes;
		len -= nbytes;
	}
	return total_bytes;
}
/* Receives data from a computer on the network.
 * Returns: bytes received (long int)
 */
long recv_data(sock_t *sock, void *data, long size, int flags)
{
	char *p=(char*)data;
	long nbytes;

	nbytes = recv(sock->fd, p, size, flags);
	return nbytes;
}
