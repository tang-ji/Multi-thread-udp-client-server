/*
 ============================================================================
 Name        : server.c
 Author      : Ji TANG
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : UDP server
 ============================================================================
 */

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

struct thread_data{
    int thread_id;
    char *message;
};

struct thread_data_r {
	int sock;
	struct sockaddr_in serv_addr;
	int thread_id;
	char *message;
};

void *reponse(void *threadarg) {
	struct thread_data_r *my_data =  (struct thread_data_r *) threadarg;
	int slen = sizeof(my_data->serv_addr);
	char buff[1024];
	pthread_t thread;
	struct sockaddr_in client_addr;
	recvfrom(my_data->sock, buff, 1024, 0, (struct sockaddr*)&client_addr, (socklen_t*)&slen);
    if(pthread_create(&thread, NULL, reponse, threadarg)) {
        fprintf(stderr, "Error creating thread\n");
    }
    printf("\nService %d: port %s received packet from %s:%d\n", my_data->thread_id, my_data->message, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Data: %s\n-------------------------------------------------------------" , buff);
    usleep(10000000);
    sendto(my_data->sock, buff, 1024, 0, (struct sockaddr*)&client_addr, slen);
    printf("\nService %d: Data \"%s\" replied!\n-------------------------------------------------------------", my_data->thread_id, buff);
}

void *creat(void *threadarg) {
	struct thread_data *my_data =  (struct thread_data *) threadarg;
	pthread_t thread;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
    char buff[1024];
	struct sockaddr_in serv_addr, client_addr;
    int slen = sizeof(serv_addr);
	memset((char *)&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(my_data->message));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	struct thread_data_r arg;
    arg.serv_addr = serv_addr;
    arg.sock = sock;
    arg.message = my_data->message;
    arg.thread_id = my_data->thread_id;

    printf("Service %d: port %s Waiting for data...\n", my_data->thread_id, my_data->message);
	while(1){
        fflush(stdout);
        if(pthread_create(&thread, NULL, reponse, (void *) &arg)) {
	        fprintf(stderr, "Error creating thread\n");
	    }
    }
}

int main(int argc, char const *argv[]){
	int n = argc - 1;
	pthread_t thread[n];
	struct thread_data td[n];

	for(int i = 0; i < n; i++) {
		td[i].thread_id = i+1;
        td[i].message = (char *)argv[i+1];
		if(pthread_create(&thread[i], NULL, creat, (void *) &td[i])) {
	        fprintf(stderr, "Error creating thread\n");
	        return 1;
	    }
	}
	
	for(int i = 0; i < n; i++) {
    	pthread_join(thread[i], NULL);
    }

    return 0;
}
