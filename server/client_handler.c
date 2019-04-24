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

	// Variables that will be sent through the communications
	int user_id;
	char filename[500];
	char path[500];

	// Cast the args back to their type, stored in external header file as included above
	client_thread_args_t *client_thread_args = (client_thread_args_t*)arg;

	// Get the client's socket instance from the struct
	int client_socket = client_thread_args->client_sock;
	
	// The first interaction using this somewhat "custom" protocol is to firstly ask for the username of the user. So prompt that.
	while(1) {
		// 1. We want the uid first
		write(client_socket, "uid", 500);

		// Receive the username
		READSIZE = recv(client_socket, client_message, 500, 0);
		
		recordLog("Recived UID:");	
		recordLog(client_message);
		
		// Convert the users uid to an int
		user_id = atoi(client_message);
		
		// 2. Now we need to ask for the filename
		write(client_socket, "filename", 500);
		
		// Receive the filename
		READSIZE = recv(client_socket, client_message, 500, 0);
		
		recordLog("Received Filename:");
		recordLog(client_message);
		
		// Copy the filename into the local variable
		strcpy(filename, client_message);

		// 3. Now we need the path
		write(client_socket, "path", 500);

		// Receive the path
		READSIZE = recv(client_socket, client_message, 500, 0);

		recordLog("Received Path");
		recordLog(client_message);

		// Copy the path into the local variable
		strcpy(path, client_message);

		/* Now all of the back and fourth for information is complete, lets check to see if the user has access to that directory */

	}
}
