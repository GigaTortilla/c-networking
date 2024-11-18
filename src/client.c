#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("test");
    int client_sock = 0;
    unsigned int n = 0;
    char recv_buf[1024];
    struct sockaddr_in server_address;

    fputs("starting memset...", stdout);
    memset(&recv_buf, '0', sizeof(recv_buf));
    memset(&server_address, '0', sizeof(server_address));
    printf("values have been set");

    if ((client_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) return EXIT_FAILURE;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    if (inet_pton(AF_INET, "0.0.0.0", &server_address.sin_addr) <= 0) return EXIT_FAILURE;

    if (connect(client_sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) return EXIT_FAILURE;
    else printf("connection established");

    while ((n = read(client_sock, recv_buf, sizeof(recv_buf) - 1)) > 0) {
        recv_buf[n] = 0;
        if (fputs(recv_buf, stdout) == EOF) {
            printf("fputs Error!");
        }
    }

    if (n < 0) printf("read error!");
    return EXIT_SUCCESS;
}
