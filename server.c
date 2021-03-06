/* Daniel Durazo
 * Matthew Ostovarpour
 * CS460
 * Datagrams in C server
 * 11/12/15
 */

#include "header.h"

// Function prototypes
void getTime(int, struct sockaddr_in, socklen_t);
void echoMessage(int, struct sockaddr_in, socklen_t);
void dnsLookup(int, struct sockaddr_in, socklen_t);


int main( int argc, char** argv) {

	// Creating required variables for UDP socket
	struct sockaddr_in server_address, client_address;
	int server_socket;
	ssize_t recv_len;
	socklen_t client_address_len = sizeof(client_address);

	// Most buffers used will be 512 characters (BUFLEN)
	char buf[BUFLEN];

	// Store client's choice for each server loop iteration
	char choice;

	// Create a UDP socket
	if ((server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("Error creating socket.");
		exit(1);
	}

	// Zero out the structure
	memset((char *) &server_address, 0, sizeof(server_address));

	// Setting address
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

		// Initially waiting for datagram
		printf("Waiting for data...\n\n");

		// Clear out the buffer memory for safe usage each time
		memset(buf, '\0', sizeof(buf));

		//try to recieve some data, this is a blocking call, show error if something goes wrong
		if ((recv_len = recvfrom(server_socket, buf, sizeof(buf), 0, (struct sockaddr *)&client_address, &client_address_len)) == -1)
		{
			perror("Error in recvfrom");
			exit(1);
		}

		// Check for the requested service to send back to client (first char of received message)
		// 1 = Echo a message
		// 2 = URL to IP lookup
		// 3 = Get current time

		choice = buf[0];

		// Choice 1: Echo
		if(choice == '1') {

			// For each case, let a function handle what needs to be done
			echoMessage(server_socket, client_address, client_address_len);

		} 
		// Choice 2: DNS Lookup 
		else if(choice == '2') {

			// Pass in the socket and address to be able to send datagrams back to client
			dnsLookup(server_socket, client_address, client_address_len);

		} 
		// Choice 3: Get current time
		else {

			getTime(server_socket, client_address, client_address_len);
		}

	}

	close(server_socket);
	return 0;
}

void echoMessage(int server_socket, struct sockaddr_in client_address, socklen_t client_address_len) {

	// Buffer for reading message from client
	char buf[BUFLEN];
	int recv_len;

	// Clear the buffer
	memset(buf, '\0', BUFLEN);

	// Try to receive the message from the client, blocking call, show error if something goes wrong
	if ((recv_len = recvfrom(server_socket, buf, sizeof(buf), 0, (struct sockaddr *)&client_address, &client_address_len)) == -1)
	{
		perror("Error in recvfrom");
		exit(1);
	}

	// Display server side message for echo request
	printf("Client echo request: %s\n", buf);
	printf("  Sending to client: %s\n\n", buf);

	// Send the message back to the client, show error if something goes wrong
	if (sendto(server_socket, buf, recv_len, 0, (struct sockaddr *)&client_address, client_address_len) == -1)
	{
		perror("Error sending message to client\n");
		exit(1);
	}
}


void getTime(int server_socket, struct sockaddr_in client_address, socklen_t client_address_len) {

	// Set up variables to use time() and localtime()
	time_t currentTime;
	struct tm *timeInfo;
	char timeString[9];	// Holds the final time in a string format

	// Get the server's local time, format it correctly, and place in buffer
	time(&currentTime);
	timeInfo = localtime(&currentTime);
	strftime(timeString, sizeof(timeString), "%H:%M", timeInfo);

	// Display server side message for this request
	printf("Client requesting local server time\n");
	printf("Sending local time: %s\n\n", timeString);

	// Send the time message back to the client, show error if something goes wrong
	if (sendto(server_socket, timeString, sizeof(timeString), 0, (struct sockaddr *)&client_address, client_address_len) == -1)
	{
		perror("Error sending message to client\n");
		exit(1);
	}
}


void dnsLookup(int server_socket, struct sockaddr_in client_address, socklen_t client_address_len) {
	// Struct for our address info
	struct addrinfo *result, *res;
	struct sockaddr_in *h;

	// Our buffer and integer for received bytes
	char buf[BUFLEN];
	int recv_len;

	// Clear the buffer
	memset(buf, '\0', BUFLEN);

	// Try to receive the message from the client, blocking call, show error if something goes wrong
	if ((recv_len = recvfrom(server_socket, buf, sizeof(buf), 0, (struct sockaddr *)&client_address, &client_address_len)) == -1)
	{
		perror("Error in recvfrom");
		exit(1);
	}

	//Display requested URL to look up
	printf(" Client DNS lookup: %s\n", buf);

	//this is to hold the addrinfo
	int status = getaddrinfo(buf, NULL, NULL, &result);


	//This determines what to do based on if there is an error with getaddrinfo
	if(status == 0){
		//get the ip addr from the struct
		h = (struct sockaddr_in*)result->ai_addr;
		strcpy(buf, inet_ntoa(h->sin_addr));

		//we need to free the memory space
		freeaddrinfo(result);
	}else{
		//if this happens then there is an error
		perror("getaddrinfo error\n");
		strcpy(buf, "getaddrinfo error\n");
	}

	//Print out the result of looking up ip address
	printf("Sending IP address: %s\n\n", buf);

	//Now we are going to send what is in the buffer (either the error message or the IP address)
	if(sendto(server_socket, buf, sizeof(buf), 0, (struct sockaddr*)&client_address, client_address_len) == -1){
		perror("sendto");
		exit(1);
	}
}

