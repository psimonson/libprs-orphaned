/* test4.c - program to test sending and receiving data with custom
 *           loop function.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#if defined(__linux) || defined(__UNIX__)
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#ifdef __linux
#include <unistd.h>
#include <sys/socket.h>
#endif

#include "sockhelp.h"

#define ADDR "127.0.0.1"
#define PORT "8000"

/* Handle incoming connections.
 */
int handle_client(sock_t *sock)
{
	if(send_data(sock, "Hello again!\r\n", 15, MSG_NOSIGNAL) < 0)
		return 1;
	close_socket(sock);
	return 0;
}
/* Server main loop function.
 */
static int main_loop(sock_t *sock)
{
	sock_t client;
	int res;
	
	init_socket(&client, NULL);
	client = accept_socket(sock); /* accept client connection */
	if(client.errno != SOCKERR_OKAY) { /* connection failed */
		printf("Warning: Could not accept connection.\n");
		return 1;
	}
	res = handle_client(&client);
	if(!res)
		close_socket(sock);
	return res;
}
/* Entry point for test.
 */
int main()
{
	sock_t server;

	socket_startup();
	init_socket(&server, main_loop);
	if(server_socket(&server, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&server));
		socket_shutdown();
		return 1;
	}
	if(!fork()) {
		sock_t client;
		char s[32];
		int retry,bytes;
		
		retry = 3;
		socket_startup();
		init_socket(&client, NULL);
		while(retry && !client_socket(&client, ADDR, PORT)) {
			if(client.errno == SOCKERR_OKAY) break;
			usleep(1000000);
			retry--;
		}
		if(!retry && client.errno != SOCKERR_OKAY) {
			fprintf(stderr, "Error: %s\n",
				get_error_socket(&client));
			socket_shutdown();
			return 1;
		}
		printf("Client received: ");
		while((bytes = recv_data(&client, s, sizeof(s),
				MSG_NOSIGNAL)) > 0) {
			s[bytes] = 0;
			printf("%s", s);
			fflush(stdout);
		}
		close_socket(&client);
		if(bytes < 0) {
			printf("\nError: Cannot receive data.\n");
			socket_shutdown();
			return 1;
		}
		printf("Test finished successfully.\n");
		socket_shutdown();
		return 0;
	}
	return loop_socket(&server, SOCKRUN_LOOP);
}
