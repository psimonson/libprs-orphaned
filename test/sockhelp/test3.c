/**
 * @file test3.c
 * @author Philip R. Simonson
 * @date 8 June 2019
 * @brief Program to test loop default function.
 *
 * Default loop function just tells you to implement a real
 * loop function.
 */

#include <stdio.h>
#include "sockhelp.h"

#define PORT "8000"

int main()
{
	sock_t *server;

	socket_startup();
	if(server_socket(&server, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(server));
		socket_shutdown();
		return 1;
	}
	destroy_socket(server);
	socket_shutdown();
	return 0;
}

