CC=gcc

all: C_socketServer.c
	$(CC) C_socketServer.c -o C_socketServer

.PHONY: clean
clean:
	rm -f C_socketServer
