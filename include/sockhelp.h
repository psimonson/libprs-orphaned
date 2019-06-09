/* sockhelp.h - Structs, functions definitions, and other stuff for
 *              my sockhelp library.
 ********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/06
 ********************************************************************
 */

#define PRS_SOCKHELP_H

#ifdef __linux
typedef int SOCKET;
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

/* Some functions for creating and closing/cleanup of sockets */
void init_socket(sock_t *sock, int (*)(sock_t *sock));
int server_socket(sock_t *sock, const char *port);
int client_socket(sock_t *sock, const char *addr, const char *port);
sock_t accept_socket(sock_t *server);
int loop_socket(sock_t *sock, int loop_type);
const char *get_error_socket(sock_t *sock);
int close_socket(sock_t *sock);

/* Helper Functions */
int bmode_socket(sock_t *sock, int);
long send_data(sock_t *sock, const void *data, long size, int flags);
long recv_data(sock_t *sock, void *data, long size, int flags);

