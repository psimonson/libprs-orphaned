/**
 * @file sockpoll.c
 * @author Philip R. Simonson
 * @date 11/23/2019
 * @brief Implementation of poll with custom socket structure in C.
 ********************************************************************
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <assert.h>

#include "sockhelp.h"
#include "sockpoll.h"

/** MACROS */
#ifndef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif

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

/* ------------------------ Global Functions ---------------------- */

/* Poll events using select.
 */
PRS_EXPORT int poll(struct pollfd *p_arr, nfds_t n_fds, int timeout)
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
