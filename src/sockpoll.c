/**
 * @file sockpoll.c
 * @author Philip R. Simonson
 * @date 11/23/2019
 * @brief Implementation of poll with custom socket structure in C.
 ********************************************************************
 */

#if defined(__linux) || defined(__UNIX)
#define _GNU_SOURCE 1
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "sockhelp.h"
#include "sockpoll.h"

/** MACROS */
#ifndef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif

static struct pollfd *_poll_fds;
static int _poll_count;

/* ----------------------- Private Functions ---------------------- */

/* Map poll() structures to file descriptor
 * sets required by select.
 */
static SOCKET map_poll_spec(struct pollfd *p_arr, nfds_t n_fds,
	fd_set *read, fd_set *write, fd_set *except)
{
	register nfds_t i;
	register struct pollfd *p_cur;
	register SOCKET max_fd;

	max_fd = INVALID_SOCKET;
	for(i = 0, p_cur = p_arr; i < n_fds; i++, p_cur++) {
		/* skip bad */
		if(get_socket(p_cur->sock) == INVALID_SOCKET)
			continue;

		if(p_cur->events & POLLIN)
			FD_SET(get_socket(p_cur->sock), read);
		if(p_cur->events & POLLOUT)
			FD_SET(get_socket(p_cur->sock), write);
		if(p_cur->events & POLLPRI)
			FD_SET(get_socket(p_cur->sock), except);

		max_fd = MAX(max_fd, get_socket(p_cur->sock));
	}
	return max_fd;
}
/* Map poll() timeout value into a select timeout.
 */
static struct timeval *map_timeout(int timeout, struct timeval *stimeout)
{
	struct timeval *result;

	assert(stimeout != (struct timeval*)NULL);
	switch(timeout) {
		case -1:
			/* A NULL timeout */
			result = (struct timeval*)NULL;
		break;
		case 0:
			/* return immediately (test) */
			stimeout->tv_sec = 0;
			stimeout->tv_usec = 0;
			result = stimeout;
		break;
		default:
			/* wait the specified number of milliseconds. */
			stimeout->tv_sec = timeout / 1000; /* get seconds */
			timeout %= 1000;		   /* remove seconds */
			stimeout->tv_usec = timeout * 1000;/* microseconds */
			result = stimeout;
		break;
	}
	return result;
}
/* Map poll() events to result events.
 */
static void map_select_results(struct pollfd *p_arr, nfds_t n_fds,
		fd_set *read, fd_set *write, fd_set *except)
{
	register unsigned long i;
	register struct pollfd *p_cur;

	for(i = 0, p_cur = p_arr; i < n_fds; i++, p_cur++) {
		/* skip bad */
		if(get_socket(p_cur->sock) == INVALID_SOCKET)
			continue;

		/* exception events take priority */
		p_cur->events = 0;
		if(FD_ISSET(get_socket(p_cur->sock), except))
			p_cur->revents |= POLLPRI;
		else if(FD_ISSET(get_socket(p_cur->sock), read))
			p_cur->revents |= POLLIN;
		if(FD_ISSET(get_socket(p_cur->sock), write))
			p_cur->revents |= POLLOUT;
	}
}
/* Handle client connection.
 */
static void default_client_connection(sock_t *sock)
{
	writef_socket(sock, "Welcome to an echo server!\r\n");
}
/* Handle client data incoming and outgoing.
 */
static int default_client_handler(sock_t *sock, int *done)
{
	char buf[BUFSIZ];
	int bytes;

	writef_socket(sock, "Enter message: ");
	if((bytes = recv_data(sock, buf, sizeof(buf), 0)) <= 0) {
		if(bytes == 0) {
			printf("Server: Client [%s] disconnected!\n",
				get_addr_socket(sock));
		} else {
			perror("recv failed");
		}
	} else {
		buf[bytes] = 0;
		if(strcmp(buf, "exit\r") == 0) {
			*done = 1;
			bytes = 0;
		}
		writef_socket(sock, "Client says: %s\r\n", buf);
	}
	return bytes;
}

/* ------------------------ Global Functions ---------------------- */

/* Get poll file descriptors. Use only with poll_multiple_socket().
 */
struct pollfd get_poll_fd(int idx)
{
	return _poll_fds[(int)((idx >= 0 && idx <= POLL_MAXCONN) ? idx : 0)];
}
/* Get poll file descriptor count. Use only with poll_multiple_socket().
 */
int get_poll_count(void)
{
	return _poll_count;
}
/* Poll events using select.
 */
int poll_socket(struct pollfd *p_arr, nfds_t n_fds, int timeout)
{
	fd_set read, write, except;
	struct timeval stime, *ptime;
	int ready, max_fd;

	FD_ZERO(&read);
	FD_ZERO(&write);
	FD_ZERO(&except);

	assert(p_arr != (struct pollfd*)NULL);
	/* map poll() file descriptor list in select() */
	max_fd = map_poll_spec(p_arr, n_fds, &read, &write, &except);
	/* map poll() timeout value in select() */
	ptime = map_timeout(timeout, &stime);
	/* make the select() call */
	ready = select(max_fd+1, &read, &write, &except, ptime);
	if(ready >= 0)
		map_select_results(p_arr, n_fds, &read, &write, &except);
	return ready;
}
/* Handle multiple connections to socket.
 */
int poll_multiple_socket(sock_t *sock, void (*func1)(sock_t*),
	int (*func2)(sock_t*, int*))
{
	struct pollfd fds[POLL_MAXCONN], old_fds[POLL_MAXCONN];
	int fd_count, bytes, i, done;
	sock_t *client;

	if(sock == NULL) return 1;

	/* Clear base file descriptor sets */
	for(i = 0; i < POLL_MAXCONN; i++) {
		old_fds[i].sock = NULL;
		old_fds[i].events = 0;
		old_fds[i].revents = 0;
	}

	/* Set initial listening socket */
	old_fds[0].sock = sock;
	old_fds[0].events = POLLIN;
	old_fds[0].revents = 0;
	fd_count = 1;

	/* Set the initial values for get functions */
	_poll_fds = old_fds;
	done = 0;

	while(!done) {
		for(i = 0; i < POLL_MAXCONN; i++)
			fds[i] = old_fds[i];

		/* Update the value for poll_count */
		_poll_count = fd_count;

		if(poll_socket(fds, fd_count, -1) < 0) {
			perror("poll failed");
			break;
		}

		for(i = 0; i < fd_count; i++) {
			if(fds[i].revents & POLLIN) {
				if(fds[i].sock == sock) {
					/* server socket */
					client = accept_socket(sock);
					if(client == NULL)
						continue;
					if(fd_count < POLL_MAXCONN) {
						printf("Server: Client [%s] connected!\n",
							get_addr_socket(client));
						if((*func1) == NULL) default_client_connection(client);
						else (*func1)(client);
						old_fds[fd_count].sock = client;
						old_fds[fd_count].events = POLLIN;
						old_fds[fd_count].revents = 0;
						fd_count++;
					} else {
						printf("Server: Too many clients already.\n");
					}
				} else {
					/* client sockets */
					if((*func2) == NULL) {
						bytes = default_client_handler(fds[i].sock, &done);
						if(done) break;
					} else {
						bytes = (*func2)(fds[i].sock, &done);
						if(done) break;
					}
					if(bytes <= 0) {
						int j;

						/* remove socket from sock set */
						destroy_socket(old_fds[i].sock);
						for(j = i; j < fd_count; j++)
							old_fds[j] = old_fds[j+1];
						fd_count--;
					} else {
						printf("Server: received %d bytes.\n",
							bytes);
					}
				}
			}
		}
	}

	/* Clear all sockets */
	for(i = 0; i < POLL_MAXCONN; i++) {
		if(old_fds[i].sock != NULL) {
			destroy_socket(old_fds[i].sock);
			old_fds[i].events = 0;
			old_fds[i].revents = 0;
		}
	}
	return 0;
#undef POLL_MAXCONN
}
