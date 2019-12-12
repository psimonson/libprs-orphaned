/**
 * @file sockpoll.h
 * @author Philip R. Simonson
 * @date 11/23/2019
 * @brief Implementation of poll event emulation in C.
 ****************************************************************************
 * @details Implementation of poll emulation in C, using select and file
 * descriptor sets.
 */

#ifndef _SOCKPOLL_H_
#define _SOCKPOLL_H_

#include "sockhelp.h"
#include "export.h"

/* Max connections at once */
#define POLL_MAXCONN 32

/* Defines for polling connections */
#define POLLIN		0x01
#define POLLPRI		0x02
#define POLLOUT		0x04
#define POLLERR		0x08
#define POLLHUP		0x10
#define POLLNVAL	0x20

/**
 * @brief Poll structure for custom socket structure.
 */
struct pollfd {
	sock_t *sock;
	short events;
	short revents;
};

typedef unsigned long nfds_t;

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get poll fd at idx. */
PRS_EXPORT struct pollfd get_poll_fd(int idx);
/** @brief Get poll count. */
PRS_EXPORT int get_poll_count(void);
/** @brief Poll events for sockets. */
PRS_EXPORT int poll_socket(struct pollfd *p_arr, nfds_t n_fds, int timeout);
/** @brief Poll server socket for multiple connections. */
PRS_EXPORT int poll_multiple_socket(sock_t *sock, void (*func1)(sock_t*),
	int (*func2)(sock_t*, int*));

#ifdef __cplusplus
}
#endif

#endif
