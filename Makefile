cc=gcc
all: udpserver udpclient tcpserver tcpclient
		gcc -o UDPEchoClient UDPEchoClient.c
udpserver: UDPEchoServer.c
	gcc -o UDPEchoServer UDPEchoServer.c
udpclient: UDPEchoClient.c
	gcc -o UDPEchoClient UDPEchoClient.c
tcpserver: TCPEchoServer.c
	gcc -o TCPEchoServer TCPEchoServer.c -lpthread
tcpclient: TCPEchoClient.c
	gcc -o TCPEchoClient TCPEchoClient.c
clean:
	rm -rf *.o
	rm UDPEchoClient
	rm UDPEchoServer
	rm TCPEchoClient
	rm TCPEchoServer
