/* This is the main server file that is responsible for opening connections on a given port and then listening for new incoming connections. Any new connections will be executed on their own thread of execution so that the server can deal with multiple clients at a given time
 * Author: Mark Barrett
 * Date: 24/04/2019
 * Git Repo: https://github.com/mark-barrett/File-Transfer-Client-Server
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "mutex.h"
#include "client_handler.h"

// As when a thread is created, only 1 argument can be sent, lets create a structure to hold the clients arguments
// Import it from seperate header file. In different file as it is going to be used across different files so its better
// to import than copy and paste across the files that need it
#include "client_thread_args.h"

// Signal handler to close socket when stopped
void signal_handler(int signal);

int main() {

	printf("Server starting...\n");
	
	// Create variables for the threading. The attributes, args, thread and client address
	pthread_attr_t pthread_attr;
	pthread_t pthread;
	socklen_t client_address_len;
	client_thread_args_t *client_thread_args;

	// Create neccessary socket variables
	int sock;
	int client_socket;
	int connection_size;
	int READ_SIZE;

	struct sockaddr_in server, client;
	char message[500];

	// Create the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);

	// If creating the socket failed
	if(sock == -1) {
		printf("Error. Could not create the socket.\n");
	} else {
		printf("Socket created successfully.\n");
	}
	
	// Assign the server variables
	server.sin_port = htons(8082);

	// Use IPV4 Protocol
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket
	if(bind(sock,(struct sockaddr*)&server, sizeof(server)) < 0) {
		printf("Error binding socket\n");
		return 1;
	} else {
		printf("Socket bound.\n");
	}

	// Listen for a connection
	listen(sock, 5);
	printf("Waiting for incoming connections from clients.\n");
	
	connection_size = sizeof(struct sockaddr_in);
	
	// Init the mutex
	if(pthread_mutex_init(&lock, NULL) != 0) {
		printf("Cannot init mutex\n");
	}

	// Loop infinitely waiting for incoming connections
	while(1) {
		// Allocate memory for the client_thread_args structure
		client_thread_args = (client_thread_args_t*)malloc(sizeof(*client_thread_args));
		
		// Accept a connection from a client
		client_socket = accept(sock,(struct sockaddr*)&client,(socklen_t*)&connection_size);
		
		// Check if successful
		if(client_socket < 0) {
			printf("Couldn't establish connection with the client\n");
		} else {
			printf("Just established a connection with the client\n");

			// Keep track of the client_socket in the client_thread_args structure
			client_thread_args->client_sock = client_socket;
			
			// It was successful, try create a thread for that client
			// pthread_create will return 0 on success
			if(pthread_create(&pthread, &pthread_attr, handle_client_thread, (void*)client_thread_args) != 0) {
				printf("Error creating thread for client.\n");
			}
		}

	}


}

void signal_handler(int signal) {
	exit(0);
}
