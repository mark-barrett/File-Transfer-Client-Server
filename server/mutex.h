/* Placing the mutex stuff in a seperate file so it can be used by both the server's main file and the client handler.
 * Author: Mark Barrett
 * Date: 24/04/2019
 * Git Repo: https://github.com/mark-barrett/File-Transfer-Client-Server
 */
#include <stdio.h>
#include <pthread.h>

#ifndef MUTEX
#define MUTEX

pthread_t tid[10];
int counter;
pthread_mutex_t lock;

#endif
