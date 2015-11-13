/*
 * Matthew Ostovarpour
 * Daniel Durazo
 * Datagrams in C client
 * 1112/15
 */

#include "header.h"

void echoServer(int, struct sockaddr_in, socklen_t); 
void dnsLookup(int, struct sockaddr_in, socklen_t);
void serverTime(int, struct sockaddr_in, socklen_t);

int main(int argc, char** argv){
	int userChoice;
	struct sockaddr_in server_address;
	int client_socket;
	socklen_t slen = sizeof(server_address);
	char buf[BUFLEN];
	char message[BUFLEN];

	if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("Socket error\n");
	    exit(1);
    }

    memset((char *)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_aton(SERVER_ADDR, &server_address.sin_addr) == 0){
    	perror("inet_aton() failed\n");
    	exit(1);
    }

    // Keep allowing user to choose, choice of 4 will exit
    while(1){
        printf("==========Welcome to the Datagram programming assignment!==========\n");
        printf("=         Enter 1 to use the echo server.                         =\n");
        printf("=         Enter 2 to use the DNS lookup service.                  =\n");
        printf("=         Enter 3 to get the server time.                         =\n");
        printf("=         Enter 4 to exit.                                        =\n");
        printf("===================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);
        
	if(userChoice == 1) { 
		// Let user send a message to the server
		echoServer(client_socket, server_address, slen); 
	}

        else if(userChoice == 2) {
		// Let user get the IP for their requested URL
		dnsLookup(client_socket, server_address, slen);
        }
        else if(userChoice == 3) {
            serverTime(client_socket, server_address, slen);
        }
        else if(userChoice == 4) return 0;
        else printf("You must enter either 1, 2, 3, or 4.\n");
    }
}

// Pass in the relevant socket/address information to be able to send datagrams
void echoServer(int client_socket, struct sockaddr_in server_address, socklen_t slen){

	// Create a character buffer (for a string) for sending & receiving
	char buf[BUFLEN];

	// Clear the buffer for safe usage (less risk of garbage data or previous long message)
	memset(buf, '\0', sizeof(buf));

	// Set the first character to let server know to echo message back
	buf[0] = '1';

	// Send the message to the server, display error if something went wrong
    	if (sendto(client_socket, buf, sizeof(buf), 0, (struct sockaddr *)&server_address, slen) == -1){
    		perror("sendto");
    		exit(1);
        }

	// Clear the buffer for safe usage (less risk of garbage data or previous long message)
	memset(buf, '\0', sizeof(buf));

	// Grab a string from the user and place it after the initial character
    	printf("Enter message: ");
    	scanf(" %[^\n]%*c", buf);

    	// Send the message to the server, display error if something went wrong
    	if (sendto(client_socket, buf, sizeof(buf), 0, (struct sockaddr *)&server_address, slen) == -1){
    		perror("sendto");
    		exit(1);
        }

	// Clear the buffer again for safe usage
        memset(buf, '\0', sizeof(buf));

        // Receive the echo'd message from the server
        if((recvfrom(client_socket, buf, sizeof(buf), 0, NULL, NULL)) == -1){
            perror("Receive error");
            exit(1);
        }

	// Print the received echo message to show it worked!
        printf("Received from server: %s\n", buf);
}

void dnsLookup(int client_socket, struct sockaddr_in server_address, socklen_t slen){

	// Create a character buffer (for a string) for sending & receiving
	char buf[BUFLEN];

	// Clear the buffer for safe usage (less risk of garbage data or previous long message)
	memset(buf, '\0', sizeof(buf));

	// Set the first character to let server know to echo message back
	buf[0] = '2';

	// Send the message to the server, display error if something went wrong
    	if (sendto(client_socket, buf, sizeof(buf), 0, (struct sockaddr *)&server_address, slen) == -1){
    		perror("Error sending to server\n");
    		exit(1);
        }

	// Clear the buffer for safe usage (less risk of garbage data or previous long message)
	memset(buf, '\0', sizeof(buf));

	// Grab a string from the user and place it after the initial character
    	printf("Enter URL to IP lookup: ");
    	scanf(" %[^\n]%*c", buf);

    	// Send the message to the server, display error if something went wrong
    	if (sendto(client_socket, buf, sizeof(buf), 0, (struct sockaddr *)&server_address, slen) == -1){
    		perror("Error sending to server");
    		exit(1);
        }

	// Clear the buffer again for safe usage
        memset(buf, '\0', sizeof(buf));

        // Receive the echo'd message from the server
        if((recvfrom(client_socket, buf, sizeof(buf), 0, NULL, NULL)) == -1){
            perror("Receive error");
            exit(1);
        }

	// Print the received echo message to show it worked!
        printf("The address resolved to: %s\n", buf);

}

void serverTime(int client_socket, struct sockaddr_in server_address, socklen_t slen){
    
	// Create a character buffer (for a string) for receiving
	char buf[BUFLEN];

	// Clear the buffer for safe usage (less risk of garbage data or previous long message)
	memset(buf, '\0', sizeof(buf));

	// Set the first character to let server know to send time back
	buf[0] = '3';

    	// Send the message to the server, display error if something went wrong
    	if (sendto(client_socket, buf, sizeof(buf), 0, (struct sockaddr *)&server_address, slen) == -1){
    		perror("Error sending time request to server");
    		exit(1);
        }

	// Clear the buffer again for safe usage
        memset(buf, '\0', sizeof(buf));

        // Receive the time message from the server
        if((recvfrom(client_socket, buf, sizeof(buf), 0, NULL, NULL)) == -1){
            perror("Receive error");
            exit(1);
        }

	// Print the received echo message to show it worked!
        printf("Time from server: %s\n", buf);

}


