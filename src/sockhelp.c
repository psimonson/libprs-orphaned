/**
 * @file sockhelp.c
 * @author Philip R. Simonson
 * @date 06 Jun 2019
 * @brief Simple socket helper library (WIP).
 * @details
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
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>

/* Headers for socket programming */
#ifdef _WIN32
#undef _WIN32_WINNT
#define WINVER 0x0700
#define _WIN32_WINNT 0x0700
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#endif

#include "sockhelp.h"
#include "unused.h"

#define BACKLOG 32       /**< Backlog define for server socket. */

/** @brief socket structure */
struct socket {
	struct sockaddr_storage addr;	/**< Socket address storage */
	SOCKET fd;			/**< Socket descriptor */
	int error;			/**< Error code storage */
};

/** @brief Internally used function: clears a socket structure. */
static void clear_socket(sock_t *sock);

/* ------------------------- Init Functions --------------------- */

/**
 * @brief Initialize sockets in Windows/Linux.
 */
int socket_startup()
{
#ifdef _WIN32
	WSADATA wsaData;
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
 * @brief Create server socket.
 *
 * Returns: 1=failure, 0=success
 */
int server_socket(sock_t **sock, const char *port)
{
	struct addrinfo hints,*servinfo,*p;
#ifdef _WIN32
	char yes='1';
#else
	int yes=1;
#endif
	int rv;

	/* create socket */
	*sock = (sock_t*)malloc(sizeof(sock_t));
	if((*sock) == NULL)
		return 1;
	clear_socket(*sock);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; /* use my IP */

	if((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all results */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if(((*sock)->fd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == INVALID_SOCKET) {
			(*sock)->error = SOCKERR_CREATE;
			continue;
		}

		if(setsockopt((*sock)->fd, SOL_SOCKET, SO_REUSEADDR, &yes,
			sizeof(int)) == SOCKET_ERROR) {
			(*sock)->error = SOCKERR_OPTION;
			close_socket(*sock);
			return 1;
		}

		if(bind((*sock)->fd, p->ai_addr, p->ai_addrlen)
			== SOCKET_ERROR) {
			(*sock)->error = SOCKERR_BIND;
			close_socket(*sock);
			continue;
		}
		break;
	}

	if(p == NULL) {
		(*sock)->error = SOCKERR_BIND;
		close_socket(*sock);
		return 1;
	}
	freeaddrinfo(servinfo);		/* all done with this */

	if(listen((*sock)->fd, BACKLOG) == SOCKET_ERROR) {
		(*sock)->fd = SOCKERR_LISTEN;
		close_socket(*sock);
		return 1;
	}
	printf("server: %s listening on port %s.\n"
		"server: waiting for connections...\n",
		get_addr_socket((*sock)), port);
	(*sock)->error = SOCKERR_OKAY;
	return 0;
}
/**
 * @brief Create a client connection.
 *
 * Returns: 1=failure, 0=success
 */
int client_socket(sock_t **sock, const char *addr, const char *port)
{
	struct addrinfo hints,*servinfo,*p;
	int rv, copied;

	copied = 0;

	/* create socket */
	*sock = (sock_t*)malloc(sizeof(sock_t));
	if((*sock) == NULL)
		return 1;
	clear_socket(*sock);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if((rv=getaddrinfo(addr, port, &hints, &servinfo)) != 0) {
		return 1;
	}

	/* loop through all results */
	for(p=servinfo; p!=NULL; p=p->ai_next) {
		if(((*sock)->fd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == INVALID_SOCKET) {
			(*sock)->error = SOCKERR_CREATE;
			continue;
		}
		if(connect((*sock)->fd, p->ai_addr, p->ai_addrlen)
				== SOCKET_ERROR) {
			(*sock)->error = SOCKERR_CONNECT;
			close_socket(*sock);
			continue;
		}
		break;
	}

	if(p==NULL) {
		(*sock)->error = SOCKERR_CONNECT;
		close_socket(*sock);
		return 1;
	} else {
		memcpy(&(*sock)->addr, &p->ai_addr, sizeof(struct sockaddr_storage));
		copied = 1;
	}
	freeaddrinfo(servinfo);
	
	if(copied) {
		printf("Client: connecting to %s\n", get_addr_socket((*sock)));
		(*sock)->error = SOCKERR_OKAY;
		return 0;
	}
	printf("Client: %s failed to connect.\n", addr);
	(*sock)->error = SOCKERR_CONNECT;
	return 1;
}
/**
 * @brief Accepts client socket from server socket.
 *
 * Returns: sock_t
 */
sock_t *accept_socket(sock_t *server)
{
	socklen_t sin_size = sizeof(struct sockaddr_storage);
	sock_t *sock;

	/* create socket */
	sock = (sock_t*)malloc(sizeof(sock_t));
	if(sock == NULL)
		return NULL;
	clear_socket(sock);
	sock->fd = accept(server->fd, (struct sockaddr*)&sock->addr,
			&sin_size);
	if(sock->fd == INVALID_SOCKET) {
		sock->error = SOCKERR_CREATE;
		return sock;
	}
	sock->error = SOCKERR_OKAY;
	return sock;
}
/**
 * @brief Destroys a socket object.
 *
 * Returns: void
 */
void destroy_socket(sock_t *sock)
{
	if(sock != NULL) {
		if(sock->fd != INVALID_SOCKET) close_socket(sock);
		if(sock != NULL) {
			clear_socket(sock);
			free(sock);
		}
	}
}
/**
 * @brief Gets current error message.
 *
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
	return errmsg[sock->error];
}
/**
 * @brief Gets current error number.
 *
 * Returns: int
 */
int get_errori_socket(sock_t *sock)
{
	if(sock->error < 0 || sock->error >= SOCKERR_COUNT)
		return SOCKERR_UNKNOWN;
	return sock->error;
}
/**
 * @brief Close socket and cleanup structure.
 *
 * Returns: 1=failure,0=success
 */
int close_socket(sock_t *sock)
{
	int res = 0;
#ifdef _WIN32
	if((res = closesocket(sock->fd))) {
		printf("Error: Could not close socket.\n");
		sock->error = SOCKERR_CLOSE;
		return res;
	}
#else
	if((res = close(sock->fd))) {
		printf("Error: Could not close socket.\n");
		sock->error = SOCKERR_CLOSE;
		return res;
	}
#endif
	sock->fd = INVALID_SOCKET;
	sock->error = SOCKERR_OKAY;
	return res;
}

/* ------------------------ Helper Functions --------------------- */

/**
 * @brief Sets whether or not it's blocking.
 *
 * Returns: true=failure, false=success
 */
int blocking_socket(sock_t *sock, int bmode)
{
#ifdef _WIN32
	long unsigned int mode = bmode ? 0 : 1;
	return ioctlsocket(sock->fd, FIONBIO, &mode);
#else
	int flags;
	flags = fcntl(sock->fd, F_GETFL);
	if(bmode)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;
	return fcntl(sock->fd, F_SETFL, flags);
#endif
}
/**
 * @brief Sends data across the network to another computer.
 *
 * Returns: bytes sent (long int)
 */
long send_data(sock_t *sock, const void *data, long size, int flags)
{
#ifdef _WIN32
	int addrlen = sizeof(struct sockaddr);
#else
	socklen_t addrlen = sizeof(struct sockaddr);
#endif
	return sendto(sock->fd, data, size, flags,
		(struct sockaddr*)&sock->addr, addrlen);
}
/**
 * @brief Receives data from a computer on the network.
 *
 * Returns: bytes received (long int)
 */
long recv_data(sock_t *sock, void *data, long size, int flags)
{
#ifdef _WIN32
	int addrlen = sizeof(struct sockaddr);
#else
	socklen_t addrlen = sizeof(struct sockaddr);
#endif
	return recvfrom(sock->fd, data, size, flags,
		(struct sockaddr*)&sock->addr, &addrlen);
}
/**
 * @brief Reverse string in place.
 */
void reverse(char *s)
{
	int i, j, c;
	for(i=0, j=strlen(s)-1; i < j; ++i,--j) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
#ifndef _WIN32
/**
 * @brief Convert decimal number into string.
 */
void itoa(int x, char *buf)
{
	int i, sign;
	if((sign = x) < 0)
		x = -x;
	i = 0;
	do {
		buf[i++] = x % 10 + '0';
	} while((x /= 10) > 0);
	if(sign < 0)
		buf[i++] = '-';
	buf[i] = '\0';
	reverse(buf);
}
#endif
/**
 * @brief Write formatted messages to socket.
 *
 * Returns: bytes written (int)
 */
int writef_socket(sock_t *sock, const char *format, ...)
{
	va_list ap;
	int written;
	va_start(ap, format);
	written = 0;
	while(*format != 0) {
		int maxrem = INT_MAX - written;
		const char *format_begun_at;

		if(format[0] != '%' || format[1] == '%') {
			int amount;
			if(format[0] == '%')
				format++;
			amount = 1;
			while(format[amount] && format[amount] != '%')
				amount++;
			if(maxrem < amount) {
				return -1;
			}
			if(send_data(sock, format, amount, 0) < 0)
				return -1;
			format += amount;
			written += amount;
		}

		format_begun_at = format++;

		if(*format == 'c') {
			char c;
			format++;
			c = (char)va_arg(ap, int);
			if(!maxrem) {
				/* TODO: Set overflow error. */
				return -1;
			}
			if(send_data(sock, &c, sizeof(c), 0) < 0)
				return -1;
			written++;
		} else if(*format == 'd') {
			char buf[32];
			int len, x;
			format++;
			x = (int)va_arg(ap, int);
#ifdef _WIN32
			itoa(x, buf, sizeof(buf));
#else
			itoa(x, buf);
#endif
			len = strlen(buf);
			if(maxrem < len) {
				/* TODO: Set overflow error. */
				return -1;
			}
			if(send_data(sock, buf, len, 0) < 0)
				return -1;
			written += len;
		} else if(*format == 's') {
			const char *s;
			int len;
			format++;
			s = (const char*)va_arg(ap, const char*);
			len = strlen(s);
			if(maxrem < len) {
				/* TODO: Set overflow error. */
				return -1;
			}
			if(send_data(sock, s, len, 0) < 0)
				return -1;
			written += len;
		} else {
			int len;
			format = format_begun_at;
			len = strlen(format);
			if(maxrem < len) {
				/* TODO: Set overflow error. */
				return -1;
			}
			if(send_data(sock, format, len, 0) < 0)
				return -1;
			written += len;
			format += len;
		}
	}
	va_end(ap);
	return written;
}
/**
 * @brief Get address from socket.
 */
const char *get_addr_socket(sock_t *s)
{
	static char addr[32];
	memset(addr, 0, sizeof(addr));
	sprintf(addr, "%ld.%ld.%ld.%ld",
#if __BIG_ENDIAN__ == __LITTLE_ENDIAN
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF000000),
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF0000) >> 8,
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF00) >> 16,
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF) >> 24);
#else
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF),
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF00) >> 8,
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF0000) >> 16,
	(long)(((struct sockaddr_in*)&s->addr)->sin_addr.s_addr & 0xFF000000) >> 24);
#endif
	return (const char *)addr;
}
/**
 * @brief Clears socket structure, makes fd = INVALID_SOCKET
 */
static void clear_socket(sock_t *sock)
{
	memset(sock, 0, sizeof(sock_t));
	sock->fd = INVALID_SOCKET;
}
/**
 * @brief Get socket descriptor.
 */
SOCKET get_socket(sock_t *sock)
{
	return (sock != NULL ? sock->fd : INVALID_SOCKET);
}
/**
 * @brief Get address information from socket.
 */
void *get_addr_info(sock_t *sock)
{
	return (void*)&sock->addr;
}
/**
 * @brief Set socket descriptor.
 */
sock_t *new_socket(void *addrinfo, SOCKET fd)
{
	sock_t *sock;
	sock = (sock_t*)malloc(sizeof(sock_t));
	if(sock == NULL) return NULL;
	if(addrinfo != NULL) memcpy(&sock->addr, addrinfo, sizeof(sock->addr));
	else memset(&sock->addr, 0, sizeof(sock->addr));
	sock->fd = fd;
	sock->error = SOCKERR_OKAY;
	return sock;
}
