cc=gcc
all: server client
		gcc -o UDPEchoClient UDPEchoClient.c
server: UDPEchoServer.c
	gcc -o UDPEchoServer UDPEchoServer.c
client: UDPEchoClient.c
	gcc -o UDPEchoClient UDPEchoClient.c
clean:
	rm -rf *.o
	rm UDPEchoClient
	rm UDPEchoServer
