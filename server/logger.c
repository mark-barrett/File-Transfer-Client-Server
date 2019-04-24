/* This C file is used to handle logging of the actions that are carried on within the Code Deployment and Backup Daemon.
 * Author: Mark Barrett
 * Date: 12/03/2019
 * Git Repo: https://github.com/mark-barrett/Code-Backup-and-Deployment-Daemon
 */
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int recordLog(char log[]) {
	// Get today as time
	time_t now = time(NULL);

	// The time as a string
	char log_message[100+strlen(log)];
	
	// Add it to the start of the log
	strftime(log_message, 50+strlen(log), "[%Y-%m-%d %H:%M:%S File Transfer Server] ", localtime(&now));
	
	// Move to the root directory so we can move freely
	if(chdir("/") < 0) {
		printf("Cannot get to root, try running with sudo\n");
		exit(EXIT_FAILURE);
	}

	// Check to see if the log directory exists.
	DIR* dir = opendir("/var/log/file-transfer-server");

	if(ENOENT == errno) {
		printf("Log directory not found, creating it\n");

		if(mkdir("/var/log/file-transfer-server", 0777) == -1) {
			printf("Error creating log folder.\n");
		}
	}
	
	// If we arrived here we have one way or another created a log foler or
	// it already exists.
	// Open the file in a+ mode which is reading and appending
	// so we don't overwrite the file each time theres a new log. If it doesn't exist this will be created.
	FILE *log_file = fopen("/var/log/file-transfer-server/main.log", "a+");
	
	// Concat the time and the log
	strcat(log_message, log);

	// Concat a new line character to the end of the log
	strcat(log_message, "\n");

	// Place the log into the file.
	fputs(log_message, log_file);

	// Close the log file
	fclose(log_file);
}
