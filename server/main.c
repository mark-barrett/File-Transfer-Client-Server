/* This is the main server file that is responsible for opening connections on a given port and then listening for new incoming connections. Any new connections will be executed on their own thread of execution so that the server can deal with multiple clients at a given time
 * Author: Mark Barrett
 * Date: 24/04/2019
 * Git Repo: https://github.com/mark-barrett/File-Transfer-Client-Server
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "logger.h"
#include "client_handler.h"

// As when a thread is created, only 1 argument can be sent, lets create a structure to hold the clients arguments
typedef struct client_thread_args_t {
	int socket_fd;
	struct sockaddr_in client_address;
	char username[100];
} client_thread_args_t;


int main() {

	recordLog("Server starting...");
	
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
		recordLog("Error. Could not create the socket.");
	} else {
		recordLog("Socket created successfully.");
	}
	
	// Assign the server variables
	server.sin_port = htons(8082);

	// Use IPV4 Protocol
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket
	if(bind(sock,(struct sockaddr*)&server, sizeof(server)) < 0) {
		recordLog("Error binding socket");
		return 1;
	} else {
		recordLog("Socket bound.");
	}

	// Listen for a connection
	listen(sock, 3);
	recordLog("Waiting for incoming connections from clients");
	
	connection_size = sizeof(struct sockaddr_in);

	// Loop infinitely waiting for incoming connections
	while(1) {
		// Accept a connection from a client
		client_socket = accept(sock,(struct sockaddr*)&client,(socklen_t*)&connection_size);

		// Check if successful
		if(client_socket < 0) {
			recordLog("Couldn't establish connection with client.");
		} else {
			recordLog("Connection from client established.");

			// It was successful, try create a thread for that client
			// pthread_create will return 0 on success
			if(pthread_create(&pthread, &pthread_attr, handle_client_thread, (void*)client_thread_args) != 0) {
				recordLog("Error creating thread for client.");
			} else {
				recordLog("Succesfully created thread for client.");
			}
		}
	}


}
