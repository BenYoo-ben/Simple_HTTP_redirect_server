#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define __CAPTIVE_PORTAL_PORT__ 55551
#define __CAPTIVE_PORTAL_WAIT_MAX__ 10
#define __CAPTIVE_PORTAL_MTU__ 1500000

static char *global_captive_portal_url;
static unsigned int global_captive_portal_port;

void* captive_portal_handler(void *sock_arg);
void captive_portal_redirect_socket_to_url(int socket, const char *url);
