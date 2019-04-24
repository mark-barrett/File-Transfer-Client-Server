/* Placing this structure in a seperate file as it is easier to import as it will be used by multiple files.
 * Author: Mark Barrett
 * Date: 24/04/2019
 * Git Repo: https://github.com/mark-barrett/File-Transfer-Client-Server
 */
#include <stdio.h>
#ifndef CLIENT_THREAD_ARGS
#define CLIENT_THREAD_ARGS

typedef struct client_thread_args_t {
	int client_sock;
	char username[100];
} client_thread_args_t;

#endif
