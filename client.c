/*
 ============================================================================
 Name        : client.c
 Author      : Ji TANG
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : UDP client
 ============================================================================
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char buff[1024];
    char message[1024];
    
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in serv_addr;
	int slen = sizeof(serv_addr);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	inet_aton(argv[1] , &serv_addr.sin_addr);
	serv_addr.sin_port = htons(atoi(argv[2]));
	bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	while(1){
		printf("Input a string: ");
        gets(buff);
		sendto(sock, buff, 1024, 0, (struct sockaddr*)&serv_addr, slen);
		printf("The message has been sent to %s:%s\n", argv[1], argv[2]);

		memset(message, '\0', 1024);
		recvfrom(sock, message, 1024, 0, (struct sockaddr*)&serv_addr, (socklen_t*)&slen);
        printf("Message received from server: %s\n-----------------------------------------\n", buff);
	}
	return 0;
}