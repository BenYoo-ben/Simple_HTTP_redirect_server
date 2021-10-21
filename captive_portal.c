#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define __CAPTIVE_PORTAL_PORT__ 55555
#define __CAPTIVE_PORTAL_WAIT_MAX__ 10

pthread_mutex_t captive_portal_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t captive_portal_cond = PTHREAD_COND_INITIALIZER;

void *captive_portal_handler(void *sock_arg);

int main(){
	
	int server_socket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(server_socket < 0)
	{
		printf("server socket init fail \n");
		return 1;
	}
	
	struct sockaddr_in serv_addr;
	
	memset(&serv_addr,0,sizeof(serv_addr));

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

		int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
		if(client_socket < 0){
			printf("client socket accept fail \n");
		}

		pthread_t new_thread;


		pthread_create(&new_thread, NULL, captive_portal_handler, (void *)&client_addr);

	}

}


void *captive_portal_handler(void *sock_arg){
	int sock = *((int *)sock_arg);

	printf("sock desc : %d\n",sock);
}
