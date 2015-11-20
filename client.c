/* Daniel Durazo
 * Matthew Ostovarpour
 * CS460
 * Datagrams in C server
 * 11/12/15
 */

#include "header.h"

// Function Prototypes
void echoServer(int, struct sockaddr_in, socklen_t); 
void dnsLookup(int, struct sockaddr_in, socklen_t);
void serverTime(int, struct sockaddr_in, socklen_t);

int main(int argc, char** argv){

	// Creating required variables for UDP socket
	int userChoice;
	struct sockaddr_in server_address;
	int client_socket;
	socklen_t slen = sizeof(server_address);

	// Creating buffer for sending/receiving datagrams (512 characters, defined in header)
	char buf[BUFLEN];

	// Create the socket
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("Socket error\n");
		exit(1);
	}

	// Set up the address
	memset((char *)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);

	// Internet address manipulation?
	if (inet_aton(SERVER_ADDR, &server_address.sin_addr) == 0){
		perror("inet_aton() failed\n");
		exit(1);
	}

	// Keep allowing user to choose, choice of 4 will exit, nice visual menu for easy understanding
	while(1){
		printf("+-------- C Datagram Client --------+\n");
		printf("|     1. Echo Server                |\n");
		printf("|     2. DNS Lookup service         |\n");
		printf("|     3. Get server time.           |\n");
		printf("|     4. Quit                       |\n");
		printf("+-----------------------------------+\n");
		printf("What would you like to do: ");
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
			// Get the server time
			serverTime(client_socket, server_address, slen);
		}
		// Exit the client
		else if(userChoice == 4) return 0;

		// Invalid input/choice
		else printf("You must enter 1, 2, 3, or 4.\n\n");
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
	printf("\nEnter message to send: ");
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
	printf(" Returned from server: %s\n\n", buf);
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
	printf("\nURL to lookup: ");
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
	printf("The URL IP is: %s\n\n", buf);

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
	printf("\nThe server time is: %s\n\n", buf);

}


