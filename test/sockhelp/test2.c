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
#include <windows.h>
#endif

#include <stdio.h>
#include "sockhelp.h"

#define PORT "8000"
#define MESSAGE "Hello world test!!!\r\n"

#ifdef _WIN32
DWORD thread(LPVOID p)
{
	char buf[32];
	sock_t client2;
	int retry,bytes;

	retry = 3;
	socket_startup();
	init_socket(&client2, NULL);
	while(retry && !client_socket(&client2, "127.0.0.1", "8000")) {
		if(client2.error == SOCKERR_OKAY) break;
		Sleep(2);
		retry--;
	}
	if(retry == 0 && client2.error != SOCKERR_OKAY) {
		fprintf(stderr, "Error: %s\n",
			get_error_socket(&client2));
		return 1;
	}
	printf("Client received: ");
	while((bytes = recv_data(&client2, buf, sizeof(buf), 0)) > 0) {
		buf[bytes] = 0;
		printf("%s", buf);
		fflush(stdout);
	}
	printf("Done with test... could connect!\n");
	close_socket(&client2);
	return 0;
}
#endif
int main()
{
	sock_t server,client;
#ifdef _WIN32
	HANDLE mthread;
#endif
	socket_startup();
	init_socket(&server, NULL);
	init_socket(&client, NULL);
	if(server_socket(&server, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&server));
		socket_shutdown();
		return 1;
	}
#ifdef _WIN32
	mthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&thread,
		(LPVOID)&thread, 0, NULL);
#else
	if(!fork()) {
		char buf[32];
		sock_t client2;
		int retry,bytes;

		retry = 3;
		socket_startup();
		init_socket(&client2, NULL);
		while(retry && !client_socket(&client2, "127.0.0.1", "8000")) {
			if(client2.error == SOCKERR_OKAY) break;
			usleep(1000000);
			retry--;
		}
		if(retry == 0 && client2.error != SOCKERR_OKAY) {
			fprintf(stderr, "Error: %s\n",
				get_error_socket(&client2));
			socket_shutdown();
			return 1;
		}
		printf("Client received: ");
		while((bytes = recv_data(&client2, buf, sizeof(buf), 0)) > 0) {
			buf[bytes] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
		printf("Done with test... could connect!\n");
		close_socket(&client2);
		socket_shutdown();
		return 0;
	}
#endif
	printf("Waiting for client...\n");
	client = accept_socket(&server);
	if(client.error != SOCKERR_OKAY) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&server));
		close_socket(&server);
		socket_shutdown();
		return 1;
	}
	printf("Sending data... ");
	if(send_data(&client, MESSAGE, sizeof(MESSAGE), 0) < 0) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&client));
		close_socket(&client);
		close_socket(&server);
		socket_shutdown();
		return 0;
	}
#ifdef _WIN32
	WaitForSingleObject(mthread, 0);
#endif
	printf("done.\n");
	close_socket(&client);
	close_socket(&server);
	socket_shutdown();
	return 0;
}

