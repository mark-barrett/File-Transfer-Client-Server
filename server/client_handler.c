/* This is C file that is responsible for handling interactions with the client in its own thread.
 * Author: Mark Barrett
 * Date: 24/04/2019
 * Git Repo: https://github.com/mark-barrett/File-Transfer-Client-Server
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "logger.h"
#include "client_thread_args.h"

void *handle_client_thread(void *arg) {
	
	// Define string for the client's messages
	char client_message[500];

	// And a readsize
	int READSIZE;

	// Cast the args back to their type, stored in external header file as included above
	client_thread_args_t *client_thread_args = (client_thread_args_t*)arg;

	// Get the client's socket instance from the struct
	int client_socket = client_thread_args->client_sock;
	
	recordLog("Requesting Username");
		
	// The first interaction using this somewhat "custom" protocol is to firstly ask for the username of the user. So prompt that.
	while(1) {
		// We want the uid first
		write(client_socket, "uid", strlen("uid"));

		// Receive the username
		READSIZE = recv(client_socket, client_message, 500, 0);
		
		// Convert the users uid to an int
		int user_id = atoi(client_message);

		printf("Client sent: %d", user_id);

	}
}
