/* This is the main file for the client which is responsible for establishing a connection to the server for the file transfer.
 * Author: Mark Barrett
 * Date: 24/04/2019
 * Git Repo: https://github.com/mark-barrett/File-Transfer-Client-Server
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {

	// Make sure a file name was specified
	if(argc == 2) {
		printf("A path needs to be provided like this: ./transferFile [filename] [path-on-server]\n");
		return 0;
	}
	else if(argc < 2) {
		printf("A filename needs to be given for transfer, like this: /transferFile [filename] [path-on-server]\n");
		return 0;
	}

	// Set the filename and the path from the command line arguments
	char filename[200];
	char path[200];

	// Copy them in
	strcpy(filename, argv[1]);
	strcpy(path, argv[2]);

	// If we are here there was a filename given.
	// Create required socket variables
	int sock;
	int READSIZE;
	struct sockaddr_in server;
       	char client_message[500];
	char server_message[500];

	// Create the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sock == -1) {
		printf("Error creating the socket.\n");
	}

	// Set the socket variables.
	// Port
	server.sin_port = htons(8082);
	// Address, in this case localhost/127.0.0.1
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	// IPV4 Protocol
	server.sin_family = AF_INET;

	// Try connect to the server
	if(connect(sock,(struct sockaddr*)&server, sizeof(server)) < 0) {
		printf("Cannot connect to the file transfer server as it is not running or is on another port/address.\n");
		return 1;
	}

	printf("Conntected to the file transfer server.\n");
	
	// Infinitely loop for communication
	while(1) {
		// The server is dictating the communications, so it will ask for different items in order and the client must return them.
		if(recv(sock, server_message, 500, 0) < 0) {
			printf("Error receiving message from server");
		} else {
			if(strcmp(server_message, "uid") == 0) {
				// Need to convert the uid (int) for transfer over sockets
				char uid_as_string[20];

				sprintf(uid_as_string, "%d", getuid());

				printf("%s", uid_as_string);

				printf("%s", uid_as_string);

				// Send it on
				send(sock, uid_as_string, sizeof(uid_as_string), 0);
			} else if(strcmp(server_message, "filename") == 0) {
				// Send the filename
				send(sock, filename, sizeof(filename), 0);
			} else if(strcmp(server_message, "path") == 0) {
				send(sock, path, sizeof(path), 0);
			}
		}


		printf("Howya");
	}	

	return 0;
}
