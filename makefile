CC=gcc
CFLAGS=-I.

all: tcpclient tcpserver udpclient udpserver helloworld

tcpclient : tcpclient.c
	$(CC) tcpclient.c -o tcpclient $(CFLAGS)
	
tcpserver : tcpserver.c
	$(CC) tcpserver.c -o tcpserver $(CFLAGS)

udpclient : udpclient.c
	$(CC) udpclient.c -o udpclient $(CFLAGS)

udpserver : udpserver.c
	$(CC) udpserver.c -o udpserver $(CFLAGS)

helloworld : helloworld.c
	$(CC) helloworld.c -o helloworld $(CFLAGS)

clean :
	rm tcpclient tcpserver udpclient udpserver helloworld