int main(void){
	struct sockaddr_in server_address;
	int client_socket;
	socklen_t slen = sizeof(server_address);
	char buf[BUFLEN];
	char message[BUFLEN];

	if (client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
				 perror("socket");
				 exit(1);
             }
    memset((char *)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_aton(SERVER, &server_address.sin_addr) == 0){
    	fprintf(stderr, "inet_aton() failed\n");
    	exit(1);
    }

    while(!0){
    	printf("Enter message: ");
    	gets(message);

    	//sends the message
    	if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_address, slen) == -1){
    		perror("sendto"(;
    		exit(1);
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFLEN);
    }
}