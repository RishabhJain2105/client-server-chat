#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "rsa_utils.h"

#define PORT 8080
#define BUF_SIZE 4096

int sock;
RSA* server_rsa_pub;

void* recv_thread(void* arg) {
    unsigned char buffer[BUF_SIZE];
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int valread = recv(sock, buffer, BUF_SIZE, 0);
        if (valread <= 0) break;
        buffer[valread] = '\0';
        printf("\n[Broadcast] %s\n> ", buffer);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;
    char message[1024];
    unsigned char encrypted[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    unsigned char buffer[BUF_SIZE];
    int pub_len = recv(sock, buffer, BUF_SIZE, 0);
    BIO* bio = BIO_new_mem_buf(buffer, pub_len);
    server_rsa_pub = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);

    printf("Connected. Type message or /quit to exit:\n> ");

    pthread_t tid;
    pthread_create(&tid, NULL, recv_thread, NULL);

    while (1) {
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0;

        if (strcmp(message, "/quit") == 0) break;

        rsa_encrypt(server_rsa_pub, (unsigned char*)message, encrypted);
        send(sock, encrypted, RSA_size(server_rsa_pub), 0);
        printf("> ");
        fflush(stdout);
    }

    close(sock);
    RSA_free(server_rsa_pub);
    return 0;
}
