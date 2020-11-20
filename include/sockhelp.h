/**
 * @file sockhelp.h
 * @author Philip R. Simonson
 * @date   2019/06/06
 * @brief Simple wrapper library for Sockets in Linux/Windows.
 ********************************************************************
 * @details Simple socket library for Linux/Unix. It works with
 * Windows now, will be extending in the future.
 ********************************************************************
 */

#ifndef PRS_SOCKHELP_H
#define PRS_SOCKHELP_H

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#undef _WIN32_WINNT
#define WINVER 0x0700
#define _WIN32_WINNT 0x0700
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET SOCKET_ERROR
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#endif

/** @brief socket type definition */
typedef struct socket sock_t;

/** @brief error enum */
enum socket_error {
	SOCKERR_UNKNOWN = -1,		/**< Unknown error */
	SOCKERR_OKAY,			/**< No error */
	SOCKERR_CREATE,			/**< Creation error */
	SOCKERR_OPTION,			/**< Option error */
	SOCKERR_LISTEN,			/**< Listen error */
	SOCKERR_BIND,			/**< Bind error */
	SOCKERR_CONNECT,		/**< Connect error */
	SOCKERR_CLOSE,			/**< Close error */
	SOCKERR_COUNT			/**< How many errors */
};

/** @brief Initialise the socket library. */
PRS_EXPORT int socket_startup();
/** @brief Shutdown and cleanup the socket library. */
PRS_EXPORT int socket_shutdown();
/** @brief Create a server socket and start listening. */
PRS_EXPORT int server_socket(sock_t **sock, const char *port);
/** @brief Create a client socket and connect. */
PRS_EXPORT int client_socket(sock_t **sock, const char *addr, const char *port);
/** @brief Accept a client socket from a server socket. */
PRS_EXPORT sock_t *accept_socket(sock_t *server);
/** @brief Destroy a socket and cleanup. */
PRS_EXPORT void destroy_socket(sock_t *sock);
/** @brief Get error from socket (const char *). */
PRS_EXPORT const char *get_error_socket(sock_t *sock);
/** @brief Get error from socket (int). */
PRS_EXPORT int get_errori_socket(sock_t *sock);
/** @brief Closes a socket connection. */
PRS_EXPORT int close_socket(sock_t *sock);
/** @brief Gets address from socket. */
PRS_EXPORT const char *get_addr_socket(sock_t *sock);
/** @brief Gets socket descriptor from socket object. */
PRS_EXPORT SOCKET get_socket(sock_t *sock);
/** @brief Gets address information from socket object. */
PRS_EXPORT void *get_addr_info(sock_t *sock);
/** @brief Sets socket descriptor to socket object. */
PRS_EXPORT sock_t *new_socket(void *addrinfo, SOCKET fd);
/** @brief Does nothing right now. */
PRS_EXPORT int blocking_socket(sock_t *sock, int);
/** @brief Sends data to a socket. */
PRS_EXPORT long send_data(sock_t *sock, const void *data, long size, int flags);
/** @brief Receives data from a socket. */
PRS_EXPORT long recv_data(sock_t *sock, void *data, long size, int flags);
/** @brief Write formatted data to socket. */
PRS_EXPORT int writef_socket(sock_t *sock, const char *msg, ...);

#ifdef __cplusplus
}
#endif

#endif
