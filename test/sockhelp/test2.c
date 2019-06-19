/* test2.c - program to test sending and receiving data from server.
 **********************************************************************
 * Author: Philip R. Simonson
 * Date  : 2019/06/08
 **********************************************************************
 */

#ifdef __linux
#define _XOPEN_SOURCE 600
#include <unistd.h>
#include <sys/socket.h>
#endif

#include <stdio.h>
#include "sockhelp.h"

#define PORT "8000"
#define MESSAGE "Hello world test!!!\r\n"

int main()
{
	sock_t server,client;

	socket_startup();
	init_socket(&server, NULL);
	init_socket(&client, NULL);
	if(server_socket(&server, PORT)) {
		fprintf(stderr, "Error: %s\n", get_error_socket(&server));
		socket_shutdown();
		return 1;
	}
	if(!fork()) {
		char buf[32];
		sock_t client2;
		int retry,bytes;
		
		retry = 3;
		socket_startup();
		init_socket(&client2, NULL);
		while(retry && !client_socket(&client2, "127.0.0.1", "8000")) {
			if(client2.errno == SOCKERR_OKAY) break;
			usleep(1000000);
			retry--;
		}
		if(retry == 0 && client2.errno != SOCKERR_OKAY) {
			fprintf(stderr, "Error: %s\n",
				get_error_socket(&client2));
			socket_shutdown();
			return 1;	
		}
		printf("Client received: ");
		while((bytes = recv_data(&client2, buf, sizeof(buf),
				MSG_NOSIGNAL)) > 0) {
			buf[bytes] = 0;
			printf("%s", buf);
			fflush(stdout);
		}
		printf("Done with test... could connect!\n");
		close_socket(&client2);
		socket_shutdown();
		return 0;
	}
	printf("Waiting for client...\n");
	client = accept_socket(&server);
	if(client.errno != SOCKERR_OKAY) {
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
	printf("done.\n");
	close_socket(&client);
	close_socket(&server);
	socket_shutdown();
	return 0;
}

