/* test3.c - program to test loop default function.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#include <stdio.h>
#include <sys/socket.h>
#include "sockhelp.h"

#define PORT "8000"

int main()
{
	sock_t server;

	socket_startup();
	init_socket(&server, NULL);
	if(server_socket(&server, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&server));
		socket_shutdown();
		return 1;
	}
	loop_socket(&server, SOCKRUN_LOOP);
	close_socket(&server);
	socket_shutdown();
	return 0;
}

