all: server client

server: server.c rsa_utils.c rsa_utils.h
	gcc server.c rsa_utils.c -o server -lssl -lcrypto

client: client.c rsa_utils.c rsa_utils.h
	gcc client.c rsa_utils.c -o client -lssl -lcrypto

clean:
	rm -f server client
