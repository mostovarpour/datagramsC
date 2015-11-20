make:
	gcc client.c -o clientOut
	gcc server.c -o serverOut

clean:
	rm clientOut
	rm serverOut
