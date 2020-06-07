/* test4.c - program to test sending and receiving data with custom
 *           loop function.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#if defined(__linux) || defined(__UNIX)
#define _GNU_SOURCE 1
#include <unistd.h>
#include <sys/socket.h>
#else
#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#endif

#include <stdio.h>
#include "sockhelp.h"
#include "unused.h"

#define ADDR "127.0.0.1"
#define PORT "8000"

/* Handle incoming connections.
 */
int handle_client(sock_t *sock)
{
	if(send_data(sock, "Hello again!\r\n", 15, 0) < 0)
		return 1;
	destroy_socket(sock);
	return 0;
}
/* Server main loop function.
 */
static int main_loop(sock_t *sock)
{
	sock_t *client;
	int res;

	client = accept_socket(sock); /* accept client connection */
	if(get_errori_socket(client) != SOCKERR_OKAY) { /* connection failed */
		printf("Warning: Could not accept connection.\n");
		return 1;
	}
	res = handle_client(client);
	if(!res)
		close_socket(sock);
	return res;
}
#ifdef _WIN32
DWORD thread(LPVOID p)
{
	sock_t *client;
	char s[32];
	int retry,bytes;

	UNUSED(p);
	retry = 3;
	socket_startup();
	while(retry && !client_socket(&client, ADDR, PORT)) {
		if(get_errori_socket(client) == SOCKERR_OKAY) break;
		Sleep(2);
		retry--;
	}
	if(!retry && get_errori_socket(client) != SOCKERR_OKAY) {
		fprintf(stderr, "Error: %s\n",
			get_error_socket(client));
		socket_shutdown();
		return 1;
	}
	printf("Client received: ");
	while((bytes = recv_data(client, s, sizeof(s), 0)) > 0) {
		s[bytes] = 0;
		printf("%s", s);
		fflush(stdout);
	}
	destroy_socket(client);
	if(bytes < 0) {
		printf("\nError: Cannot receive data.\n");
		socket_shutdown();
		return 1;
	}
	printf("Test finished successfully.\n");
	socket_shutdown();
	return 0;
}
#endif
/* Entry point for test.
 */
int main()
{
	sock_t *server;

	socket_startup();
	if(server_socket(&server, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(server));
		socket_shutdown();
		return 1;
	}
#ifdef _WIN32
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&thread,
		(LPVOID)server, 0, NULL);
#else
	if(!fork()) {
		sock_t *client;
		char s[32];
		int retry, bytes;
		
		retry = 3;
		socket_startup();
		while(retry && !client_socket(&client, ADDR, PORT)) {
			if(get_errori_socket(client) == SOCKERR_OKAY) break;
			usleep(1000000);
			retry--;
		}
		if(!retry && get_errori_socket(client) != SOCKERR_OKAY) {
			fprintf(stderr, "Error: %s\n",
				get_error_socket(client));
			socket_shutdown();
			return 1;
		}
		printf("Client received: ");
		while((bytes = recv_data(client, s, sizeof(s), 0)) > 0) {
			s[bytes] = 0;
			printf("%s", s);
			fflush(stdout);
		}
		destroy_socket(client);
		if(bytes < 0) {
			printf("\nError: Cannot receive data.\n");
			socket_shutdown();
			return 1;
		}
		printf("Test finished successfully.\n");
		socket_shutdown();
		return 0;
	}
#endif
	return main_loop(server);
}

