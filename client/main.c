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
	if(argc != 2) {
		printf("A filename needs to be given for transfer.\n");
		return 0;
	}

	// If we are here there was a filename given.
	// Create required socket variables
	int sock;
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
	

	return 0;
}
