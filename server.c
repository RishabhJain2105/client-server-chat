#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "rsa_utils.h"

#define PORT 8080
#define MAX_CLIENTS 10
#define BUF_SIZE 4096

int main() {
    int server_fd, new_socket, client_sockets[MAX_CLIENTS], max_sd, activity, valread;
    struct sockaddr_in address;
    fd_set readfds;
    unsigned char buffer[BUF_SIZE], decrypted[BUF_SIZE];
    RSA* rsa_keypair = create_rsa_keypair();
    unsigned char public_key_pem[BUF_SIZE];
    BIO* bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(bio, rsa_keypair);
    int pub_len = BIO_read(bio, public_key_pem, BUF_SIZE);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    for (int i = 0; i < MAX_CLIENTS; i++) client_sockets[i] = 0;

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
            if (client_sockets[i] > 0)
                FD_SET(client_sockets[i], &readfds);

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, NULL, NULL);
            send(new_socket, public_key_pem, pub_len, 0);

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("New client connected: socket %d\n", new_socket);
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sd = client_sockets[i];
            if (sd > 0 && FD_ISSET(sd, &readfds)) {
                valread = read(sd, buffer, BUF_SIZE);
                if (valread <= 0) {
                    close(sd);
                    client_sockets[i] = 0;
                    printf("Client disconnected.\n");
                } else {
                    rsa_decrypt(rsa_keypair, buffer, decrypted);

                    // Null terminate decrypted string
                    decrypted[valread] = '\0';
                    printf("Received: %s\n", decrypted);

                    // Broadcast decrypted message
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (client_sockets[j] != 0 && client_sockets[j] != sd) {
                            send(client_sockets[j], decrypted, strlen((char*)decrypted), 0);
                        }
                    }
                }
            }
        }
    }

    RSA_free(rsa_keypair);
    return 0;
}
