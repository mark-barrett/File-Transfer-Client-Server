/* This is C file that is responsible for handling interactions with the client in its own thread.
 * Author: Mark Barrett
 * Date: 24/04/2019
 * Git Repo: https://github.com/mark-barrett/File-Transfer-Client-Server
*/
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "mutex.h"
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
		
		// Convert the users uid to an int
		user_id = atoi(client_message);
		
		// 2. Now we need to ask for the filename
		write(client_socket, "filename", 500);
		
		// Receive the filename
		READSIZE = recv(client_socket, client_message, 500, 0);
		
		// Copy the filename into the local variable
		strcpy(filename, client_message);

		// 3. Now we need the path
		write(client_socket, "path", 500);

		// Receive the path
		READSIZE = recv(client_socket, client_message, 500, 0);

		// Copy the path into the local variable
		strcpy(path, client_message);
		
		// Lock the mutex
		pthread_mutex_lock(&lock);

		/* Now all of the back and fourth for information is complete, lets check to see if the user has access to that directory */
		int ngroups = 10;
		gid_t *groups;
		gid_t supp_groups[] = {};
		int allowed_transfer = 0;		
		
		// Allocate memory for the groups
		groups = malloc(ngroups * sizeof(gid_t));
		
		// Get the username by id
		struct passwd *pws;
		pws = getpwuid(user_id);
		
		// Try get the group list
		if(getgrouplist(pws->pw_name, user_id, groups, &ngroups) == -1) {
			printf("Error getting user's groups.");
		}
		
		// Loop through all of the groups
		for(int j=0; j<ngroups; j++) {
			supp_groups[j] = groups[j];
		}
		
		uid_t uid = getuid();
		uid_t gid = getgid();
		uid_t ueid = geteuid();
		uid_t geid = getegid();
		
		// Change to that user
		setgroups(10, supp_groups);
		//setreuid(user_id, uid);
		//setregid(user_id, gid);
		seteuid(user_id);
		setegid(user_id);
		
		// Now that we have changed to that user, try and open the file
		char full_path[600];
		
		strcat(full_path, "/var/www/html/intranet/");

		strcat(full_path, path);
		
		// Concat the name of the file with the path, forward slash and name
		strcat(full_path, "/");

		strcat(full_path, filename);

		char file_buffer[512];
		
		// Try open zee file
		FILE *fp = fopen(full_path, "w");
		
		char temp[1000];

		// Check if it failed, if it did, they do not have permission
		if(fp == NULL) {

			perror(full_path);

			char temp_message[600];

			sprintf(temp_message, "Permission denied for the %s directory.", path);

			// Return the error and close
			write(client_socket, "failure-permission", 500);
			
			// Change back to root
			user_id = 0;
			//setreuid(user_id, uid);
			//setregid(user_id, gid);
			seteuid(user_id);
			setegid(user_id);

			// Unlock the mutex
			pthread_mutex_unlock(&lock);

			return NULL;			
		} else {
			// User has permission. Instruct to send the file.
			write(client_socket, "sendfile", 500);

			memset(file_buffer, 0, 512);

			int block_size, i = 0;

			// Loop through, accepting blocks

			while((block_size = recv(client_socket, file_buffer, 512, 0)) > 0) {
				
				sprintf(temp, "Data Received %d = %d\n", i, block_size);
				
				int write_sz = fwrite(file_buffer, sizeof(char), block_size, fp);

				memset(file_buffer, 0, 512);
				i++;
				
				// Check if it was the last block	
				if(block_size < 512) {
					// It was, send success message
					write(client_socket, "success", 500);
					
					// Close the file
					fclose(fp);
					
					// Change back to root
					user_id = 0;
					//setreuid(user_id, uid);
					//setregid(user_id, gid);
					seteuid(user_id);
					setegid(user_id);
					
					free(client_thread_args);

					// Close the socket
					close(client_socket);
					
					// Free full path
					strcpy(full_path, "");
					
					// Cancel thread
					pthread_cancel(pthread_self());
					
					// Unlock the mutex
					pthread_mutex_unlock(&lock);

					// Return null
					return NULL;
				}	

			}
		}
			
	}
}
