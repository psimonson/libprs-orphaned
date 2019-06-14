/* test6.c - program to test receiving data from an HTTP server with
 *           custom loop function.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "sockhelp.h"

#define ADDR "www.slackware.com"
#define PORT "80"

#define GMSG "GET / HTTP/1.0\r\nHost: " ADDR "\r\n\r\n"

/* Receives data from HTTP URL.
 */
static int client_loop(sock_t *sock)
{
	char s[32];
	int bytes;
	
	if((bytes=recv_data(sock, s, sizeof(s), MSG_NOSIGNAL)) > 0) {
		s[bytes] = 0;
		printf("%s", s);
		fflush(stdout);
	} else if(bytes == 0) {
		close_socket(sock);
		socket_shutdown();
		return 0;
	} else {
		fprintf(stderr, "Error: %s\n", get_error_socket(sock));
		close_socket(sock);
		socket_shutdown();
		return 0;
	}
	return 1;
}
/* Entry point for program.
 */
int main()
{
	sock_t client;

	socket_startup();
	init_socket(&client, client_loop);
	if(client_socket(&client, ADDR, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&client));
		return 1;
	}
	if(send_data(&client, GMSG, strlen(GMSG), MSG_NOSIGNAL) < 0) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&client));
		close_socket(&client);
		socket_shutdown();
		return 1;
	}
	return !loop_socket(&client, SOCKRUN_LOOP);
}
