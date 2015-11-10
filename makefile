make:
	gcc client.c -o clientOut
	gcc server.c -lpthread -o serverOut
