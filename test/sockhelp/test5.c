/* test5.c - program to test default loop function for client.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#ifdef __linux
#include <sys/socket.h>
#endif

#include <stdio.h>
#include "sockhelp.h"

#define ADDR "www.slackware.com"
#define PORT "80"

#define GMSG "GET / HTTP/1.0\r\nHost: " ADDR "\r\n\r\n"

int main()
{
	sock_t *client;

	socket_startup();
	if(client_socket(&client, ADDR, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(client));
		socket_shutdown();
		return 1;
	}
	destroy_socket(client);
	socket_shutdown();
	return 0;
}
