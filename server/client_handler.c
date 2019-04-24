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

		recordLog("Received Path:");
		recordLog(client_message);

		// Copy the path into the local variable
		strcpy(path, client_message);

		/* Now all of the back and fourth for information is complete, lets check to see if the user has access to that directory */
		

		// Send a prompt to send the file as the user is verified as being allowed to send the file to this directory
		write(client_socket, "sendfile", 500);
		
		// Full path
		char full_path[600];

		strcat(full_path, "/var/www/html/intranet/");

		strcat(full_path, path);

		// Concat the name of the file with the path, forward slash and name
		strcat(full_path, "/");

		strcat(full_path, filename);

		// Define the buffer, same size as the buffer in the client so we can read in blocks
		char file_buffer[512];

		// Open the file for writing/create it
		FILE *fp = fopen(full_path, "w");

		// Temp String for logs
		char temp[1000];

		// Check if that worked
		if(fp == NULL) {

			strcat(temp, "File ");
			strcat(temp, filename);
			strcat(temp, " could not be created or opened on the server.");

			return NULL;
		} else {
			// Memset to fill buffer with 0s
			memset(file_buffer, 0, 512);

			int block_size, i = 0;

			// Loop through, accepting blocks

			/* NOTE: I had to put the recv within the while loop as putting it as the condition will make the program continually check up until the condition is no longer true.
			 * i.e the loop won't be executed. But because the recv function was being called here, it was blocking the success message from being sent as the server was waiting on a message from the client. By placing it inside this is avoided.
			 */
			while(block_size > 0) {
				
				block_size = recv(client_socket, file_buffer, 512, 0);

				sprintf(temp, "Data Received %d = %d\n", i, block_size);

				int write_sz = fwrite(file_buffer, sizeof(char), block_size, fp);
				memset(file_buffer, 0, 512);
				i++;
			}

			recordLog("Transfer Complete!");

			// Send a message telling the client the transfer was successful
			write(client_socket, "success", 500);

			recordLog("Here");		
			fclose(fp);

			return NULL;
		}

	}
}
