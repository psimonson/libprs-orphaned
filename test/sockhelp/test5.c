/* test5.c - program to test default loop function for client.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#include <stdio.h>
#include <sys/socket.h>
#include "sockhelp.h"

#define ADDR "www.slackware.com"
#define PORT "80"

#define GMSG "GET / HTTP/1.0\r\nHost: " ADDR "\r\n\r\n"

int main()
{
	sock_t client;

	socket_startup();
	init_socket(&client, NULL);
	if(client_socket(&client, ADDR, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&client));
		socket_shutdown();
		return 1;
	}
	loop_socket(&client, SOCKRUN_LOOP);
	close_socket(&client);
	socket_shutdown();
	return 0;
}
