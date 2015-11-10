/*
 * Daniel Durazo
 * Matthew Ostovarpour
 * CS460
 * Datagrams in C server
 * 11/12/15
 */

#include "header.h"

#define _REENTRANT

int main( int argc, char** argv)
{
	struct sockaddr_in server_address, client_address;

	int server_socket;
	ssize_t recv_len;
	socklen_t client_address_len = sizeof(client_address);
	char buf[BUFLEN];

	//create a udp socket
	if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("Error creating socket.");
		exit(1);
    }
     //zero out the structure
     memset((char *) &server_address, 0, sizeof(server_address));

     server_address.sin_family = AF_INET;
     server_address.sin_addr.s_addr = htonl(INADDR_ANY);
     server_address.sin_port = htons(PORT);

     //bind socket to port
     if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){
     	 perror("Error binding socket.");
     	 exit(1);
     }

     //keep listening for data
     while(!0){
     	 printf("Waiting for data...\n");
     	 fflush(stdout);

     	 //try to recieve some data, this is a blocking call
     	 if ((recv_len = recvfrom(server_socket, buf, BUFLEN, 0, (struct sockaddr *)&client_address, &client_address_len)) == -1)
         {
         	 perror("recvfrom");
         	 exit(1);
         }

         //printf details of the client/peer and the data received
         //printf("Received packet from %s:%d\n", inet_ntoa(client_address.sin_addr, ntohs(client_address.sin_port)));
         //printf("Data: %s\n", buf);

         //now reply the client with the same data
         if (sendto(server_socket, buf, recv_len, 0, (struct sockaddr *)&client_address, client_address_len) == -1)
         {
         	 perror("sendto");
         	 exit(1);
         }
     }

     //close(server_socket);
     return 0;
}
