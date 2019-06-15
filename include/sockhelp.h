/* sockhelp.h - Structs, functions definitions, and other stuff for
 *              my sockhelp library.
 ********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/06
 ********************************************************************
 */

#define PRS_SOCKHELP_H

#ifdef _WIN32
#define PRS_EXPORT __declspec(dllexport)
#else
#define PRS_EXPORT extern
#endif

#ifdef __linux
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET SOCKET_ERROR
#endif

typedef struct socket sock_t;

struct socket {
	char addr[255];
	SOCKET fd;
	int errno;
	int (*loop)(sock_t *sock);
};

/* error enum */
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

/* startup and shutdown functions */
PRS_EXPORT int socket_startup();
PRS_EXPORT int socket_shutdown();
/* Some functions for creating and closing/cleanup of sockets */
PRS_EXPORT void init_socket(sock_t *sock, int (*)(sock_t *sock));
PRS_EXPORT int server_socket(sock_t *sock, const char *port);
PRS_EXPORT int client_socket(sock_t *sock, const char *addr, const char *port);
PRS_EXPORT sock_t accept_socket(sock_t *server);
PRS_EXPORT int loop_socket(sock_t *sock, int loop_type);
PRS_EXPORT const char *get_error_socket(sock_t *sock);
PRS_EXPORT int get_errori_socket(sock_t *sock);
PRS_EXPORT int close_socket(sock_t *sock);

/* Helper Functions */
PRS_EXPORT int bmode_socket(sock_t *sock, int);
PRS_EXPORT long send_data(sock_t *sock, const void *data, long size, int flags);
PRS_EXPORT long recv_data(sock_t *sock, void *data, long size, int flags);
