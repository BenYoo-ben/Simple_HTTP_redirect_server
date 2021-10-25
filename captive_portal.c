#include "captive_portal.h"
#include "get_page.h"

//mutex and cond initialized for future development, not used yet.
pthread_mutex_t captive_portal_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t captive_portal_cond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Usage : %s [redirected URL in string format] [# port] \n",
				argv[0]);
		exit(1);
	}
	//make a copy of global static variables.
	global_captive_portal_url = argv[1];
	global_captive_portal_port = atoi(argv[2]);

	//make server socket
	int server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_socket < 0) {
		printf("server socket init fail \n");
		return 1;
	}

	struct sockaddr_in serv_addr;

	//bind ip + port to server socket
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(__CAPTIVE_PORTAL_PORT__);

	if (bind(server_socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr))
			< 0) {
		printf("server socket bind fail \n");
		return 2;
	}

	//make server socket to start listen
	if (listen(server_socket, __CAPTIVE_PORTAL_WAIT_MAX__) < 0) {
		printf("server socket listen fail \n");
		return 3;
	}
	socklen_t client_addr_size = sizeof(struct sockaddr_in);

	while (1) {
		struct sockaddr_in client_addr;

		//wait for http request on desired port
		printf("waiting for client...\n");
		int client_socket = accept(server_socket,
				(struct sockaddr*) &client_addr, &client_addr_size);
		if (client_socket < 0) {
			printf("client socket accept fail \n");
		} else {
			printf("Connected.\n");
		}

		//make a pointer referencing to connected client socket.
		int *pointer_to_client_socket = malloc(sizeof(int));
		*pointer_to_client_socket = client_socket;

		pthread_t new_thread;

		//start new working thread to deal with new client
		pthread_create(&new_thread, NULL, captive_portal_handler,
				pointer_to_client_socket);

	}

}

//thread;
void* captive_portal_handler(void *sock_arg) {
	int sock = *((int*) sock_arg);
	//redirect to specific url
	captive_portal_redirect_socket_to_url(sock, global_captive_portal_url);
	//free socket
	free(sock_arg);

}

void captive_portal_redirect_socket_to_url(int socket, const char *url) {

	//basic HTTP response format, might expand with some more headers like Date.
	char *reply =
			"HTTP/1.1 200 OK\nServer: C,libcurl redirection server\nContent-Type: text/html\nConnection: close\n\n";

	/* use libcurl to get redirecting webpage.
	 *
	 * Could use HTTP 302 Location header, but if redirecting URL lives in different domain security problem arises.
	 * reference: https://stackoverflow.com/questions/17167935/302-redirect-to-a-different-domain-fails-silently
	 */
	char *redirection_page = get_page(url);

	//simple copy and making of buffer
	char *data = malloc(strlen(reply) + strlen(redirection_page) + 1);

	strcpy(data, reply);
	strcpy(data + strlen(reply), redirection_page);

	write(socket, data, strlen(data));
	printf("redirected to %s \n", url);
	free(data);
	free(redirection_page);

}

