/* test2.c - program to test sending and receiving data from server.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#if defined(__linux) || defined(__UNIX)
#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <sys/socket.h>
#else
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#endif

#include <stdio.h>
#include "sockhelp.h"
#include "unused.h"

#define PORT "8000"
#define MESSAGE "Hello world test!!!\r\n"

#ifdef _WIN32
DWORD thread(LPVOID p)
{
	char buf[32];
	sock_t *client;
	int retry,bytes;

	UNUSED(p);
	retry = 3;
	socket_startup();
	while(retry && !client_socket(&client, "127.0.0.1", "8000")) {
		if(get_errori_socket(client) == SOCKERR_OKAY) break;
		Sleep(2);
		retry--;
	}
	if(retry == 0 && get_errori_socket(client) != SOCKERR_OKAY) {
		fprintf(stderr, "Error: %s\n",
			get_error_socket(client));
		return 1;
	}
	printf("Client received: ");
	while((bytes = recv_data(client, buf, sizeof(buf), 0)) > 0) {
		buf[bytes] = 0;
		printf("%s", buf);
		fflush(stdout);
	}
	printf("Done with test... could connect!\n");
	destroy_socket(client);
	return 0;
}
#endif
int main()
{
	sock_t *server, *client;
#ifdef _WIN32
	HANDLE mthread;
#endif
	socket_startup();
	if(server_socket(&server, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(server));
		socket_shutdown();
		return 1;
	}
#ifdef _WIN32
	mthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&thread,
		(LPVOID)&thread, 0, NULL);
#else
	if(!fork()) {
		char buf[32];
		sock_t *client;
		int retry, bytes;

		retry = 3;
		socket_startup();
		while(retry && !client_socket(&client, "127.0.0.1", "8000")) {
			if(get_errori_socket(client) == SOCKERR_OKAY) break;
			usleep(1000000);
			retry--;
		}
		if(retry == 0 && get_errori_socket(client) != SOCKERR_OKAY) {
			fprintf(stderr, "Error: %s\n",
				get_error_socket(client));
			socket_shutdown();
			return 1;
		}
		printf("Client received: ");
		while((bytes = recv_data(client, buf, sizeof(buf), 0)) > 0) {
			buf[bytes] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
		printf("Done with test... could connect!\n");
		destroy_socket(client);
		socket_shutdown();
		return 0;
	}
#endif
	printf("Waiting for client...\n");
	client = accept_socket(server);
	if(get_errori_socket(client) != SOCKERR_OKAY) {
		fprintf(stderr, "Error: %s\n", get_error_socket(server));
		destroy_socket(server);
		socket_shutdown();
		return 1;
	}
	printf("Sending data... ");
	if(send_data(client, MESSAGE, sizeof(MESSAGE), 0) < 0) {
		fprintf(stderr, "Error: %s\n", get_error_socket(client));
		destroy_socket(client);
		destroy_socket(server);
		socket_shutdown();
		return 0;
	}
#ifdef _WIN32
	WaitForSingleObject(mthread, 0);
#endif
	printf("done.\n");
	destroy_socket(client);
	destroy_socket(server);
	socket_shutdown();
	return 0;
}

