#include "captive_portal.h"
#include "get_page.h"

pthread_mutex_t captive_portal_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t captive_portal_cond = PTHREAD_COND_INITIALIZER;
int main(int argc, char *argv[]){
	
	if(argc!=3){
		printf("Usage : %s [redirected URL in string format] [# port] \n",argv[0]);
		exit(1);
	}

	global_captive_portal_url = argv[1];
	global_captive_portal_port = atoi(argv[2]);

	int server_socket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(server_socket < 0)
	{
		printf("server socket init fail \n");
		return 1;
	}
	
	struct sockaddr_in serv_addr;
	
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(__CAPTIVE_PORTAL_PORT__);

	if( bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("server socket bind fail \n");
		return 2;
	}

	if( listen(server_socket, __CAPTIVE_PORTAL_WAIT_MAX__) < 0){
		printf("server socket listen fail \n");
		return 3;
	}
	socklen_t client_addr_size = sizeof(struct sockaddr_in);
	//busy wait
	while(1){
		struct sockaddr_in client_addr;

		printf("waiting for input...\n");
		int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
		if(client_socket < 0){
			printf("client socket accept fail \n");
		}else{	
			printf("Connected.\n");
		}
		
		int *pointer_to_client_socket = malloc(sizeof(int));
		*pointer_to_client_socket = client_socket;
			
		pthread_t new_thread;


		pthread_create(&new_thread, NULL, captive_portal_handler, pointer_to_client_socket);

	}

}


void *captive_portal_handler(void *sock_arg){
	int sock = *((int *)sock_arg);
	captive_portal_redirect_socket_to_url(sock,global_captive_portal_url);
	free(sock_arg);
	
}

void captive_portal_redirect_socket_to_url(int socket, const char *url){
	
	char *reply = "HTTP/1.1 200 OK\nServer: C,libcurl redirection server\nContent-Type: text/html\nConnection: close\n\n";

	char *redirection_page = get_page(url);

	char *data = malloc(strlen(reply)+strlen(redirection_page)+1);

	strcpy(data,reply);
	strcpy(data+strlen(reply),redirection_page);
	
	write(socket,data,strlen(data));
	printf("redirected to %s \n",url);
	free(data);
	free(redirection_page);

}

