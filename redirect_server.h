#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define __REDIRECT_SERVER_WAIT_MAX__ 10

static char *global_redirect_server_url;
static unsigned int global_redirect_server_port;

void* redirection_handler(void *sock_arg);
void redirect_socket_to_url(int socket, const char *url);
