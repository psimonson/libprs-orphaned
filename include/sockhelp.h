/**
 * @file sockhelp.h
 * @author Philip R. Simonson
 * @date   2019/06/06
 ********************************************************************
 * @details
 *
 * Simple socket library for Linux/Unix. I will be adding support for
 * Windows in the future.
 ********************************************************************
 */

#ifndef PRS_SOCKHELP_H
#define PRS_SOCKHELP_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define SOCKET unsigned int
#else
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET SOCKET_ERROR
#endif

typedef struct socket sock_t;

/** @brief socket structure */
struct socket {
	char addr[260];
	SOCKET fd;
	int error;
	int (*loop)(sock_t *sock);
};

/** @brief error enum */
enum socket_error {
	SOCKERR_UNKNOWN = -1,
	SOCKERR_OKAY,
	SOCKERR_CREATE,
	SOCKERR_OPTION,
	SOCKERR_LISTEN,
	SOCKERR_BIND,
	SOCKERR_CONNECT,
	SOCKERR_CLOSE,
	SOCKERR_COUNT
};

#define SOCKRUN_LOOP ((int)0x00f0)

/** @brief Initialise the socket library. */
PRS_EXPORT int socket_startup();
/** @brief Shutdown and cleanup the socket library. */
PRS_EXPORT int socket_shutdown();
/** @brief Initialise socket structure, adding a custom function. */
PRS_EXPORT void init_socket(sock_t *sock, int (*)(sock_t *sock));
/** @brief Create a server socket and start listening. */
PRS_EXPORT int server_socket(sock_t *sock, const char *port);
/** @brief Create a client socket and connect. */
PRS_EXPORT int client_socket(sock_t *sock, const char *addr, const char *port);
/** @brief Accept a client socket from a server socket. */
PRS_EXPORT sock_t accept_socket(sock_t *server);
/** @brief Main loop for my socket library. */
PRS_EXPORT int loop_socket(sock_t *sock, int loop_type);
/** @brief Get error from socket (const char *). */
PRS_EXPORT const char *get_error_socket(sock_t *sock);
/** @brief Get error from socket (int). */
PRS_EXPORT int get_errori_socket(sock_t *sock);
/** @brief Closes a socket connection. */
PRS_EXPORT int close_socket(sock_t *sock);

/** @brief Does nothing right now. */
PRS_EXPORT int bmode_socket(sock_t *sock, int);
/** @brief Sends data to a socket. */
PRS_EXPORT long send_data(sock_t *sock, const void *data, long size, int flags);
/** @brief Receives data from a socket. */
PRS_EXPORT long recv_data(sock_t *sock, void *data, long size, int flags);

#ifdef __cplusplus
}
#endif

#endif
