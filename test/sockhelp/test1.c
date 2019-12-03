/* test1.c - program to test receiving data from an HTTP server.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#include <stdio.h>
#include "sockhelp.h"

#define SITE "www.slackware.com"
#define PORT "80"

#define GETMETHOD "GET / HTTP/1.0\r\nHost: " SITE "\r\n\r\n"

int main()
{
	char buffer[1024];
	sock_t *client;
	long bytes;

	socket_startup();
	if(client_socket(&client, SITE, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(client));
		socket_shutdown();
		return 1;
	}
	if(send_data(client, GETMETHOD, sizeof(GETMETHOD), 0) < 0) {
		fprintf(stderr, "Error: %s\n", get_error_socket(client));
		destroy_socket(client);
		socket_shutdown();
		return 1;
	}
	printf("Receiving...\n");
	while((bytes = recv_data(client, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytes] = 0;
		printf("%s", buffer);
		fflush(stdout);
	}
	destroy_socket(client);
	socket_shutdown();
	return 0;
}

