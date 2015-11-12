/*
 * Matthew Ostovarpour
 * Daniel Durazo
 * Datagrams in C client
 * 1112/15
 */

#include "header.h"

void echoServer(); 
int dnsLookup(char *, char *);
void serverTime();

int main(int argc, char** argv){
    int userChoice, keepGoing;
    while(keepGoing = 1){
        printf("==========Welcome to the Datagram programming assignment!==========\n");
        printf("=         Enter 1 to use the echo server.                         =\n");
        printf("=         Enter 2 to use the DNS lookup service.                  =\n");
        printf("=         Enter 3 to get the server time.                         =\n");
        printf("=         Enter 4 to exit.                                        =\n");
        printf("===================================================================\n");
        printf("Enter your choice: ");
        scanf(" %d", &userChoice);
        if(userChoice == 1) echoServer();
        else if(userChoice == 2) {
            char hostname[100];
            char ip[100];
            printf("Enter the address you would like to lookup: ");
            scanf(" %[^\n]%*c", &hostname);
            dnsLookup(hostname, ip);
            printf("The address %s resolved to %s\n");
        }
        else if(userChoice == 3) {
            serverTime();
        }
        else if(userChoice == 4) return 0;
        else printf("You must enter either 1, 2, 3, or 4.\n");
    }
}

void echoServer(){
	struct sockaddr_in server_address;
	int client_socket;
	socklen_t slen = sizeof(server_address);
	char buf[BUFLEN];
	char message[BUFLEN];

	if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
		perror("socket");
	    exit(1);
    }
    memset((char *)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_aton(SERVER_ADDR, &server_address.sin_addr) == 0){
    	fprintf(stderr, "inet_aton() failed\n");
    	exit(1);
    }

    while(!0){
    	printf("Enter message: ");
    	gets(message);

    	//sends the message
    	if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_address, slen) == -1){
    		perror("sendto");
    		exit(1);
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFLEN);
        if((recvfrom(client_socket, buf, 512, 0, NULL, NULL)) == -1){
            perror("Receive error");
            exit(1);
        }
        printf("Received: %s\n", buf);
        memset(buf, '\0', BUFLEN);
    }
}

int dnsLookup(char *hostname, char *ip){
    int sockfd;
    struct addrinfo htoip, *servInfo, *p;
    struct sockaddr_in *h;
    int rv;

    memset(&htoip, 0, sizeof htoip);
    htoip.ai_family = AF_UNSPEC;
    htoip.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(hostname, "http", &htoip, &servInfo)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    //Loop through all of the results and connect to the first one
    for (p = servInfo; p != NULL; p = p->ai_next){
        h = (struct sockaddr_in *)p->ai_addr;
        strcpy(ip, inet_ntoa(h -> sin_addr));
    }
    freeaddrinfo(servInfo);
    return 0;
}
 void serverTime(){
    time_t currentTime;
    struct tm *timeInfo;
    char timeString[9];
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(timeString, sizeof(timeString), "%H:%M", timeInfo);
    printf("The time is %s\n", timeString);
 }
