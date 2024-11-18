#include <stdlib.h>
#include <stdio.h>
#include <sys/syslog.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

// server connection
int main(int argc, char *argv[]) {
    int listening, client_sock;
    struct sockaddr_in server_address, client;
    socklen_t client_size = sizeof(client);
    char host[NI_MAXHOST];
    char service[NI_MAXHOST];
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXHOST);

    listening = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(54000);

    int sock_bind = bind(listening, (struct sockaddr*) &server_address, sizeof(server_address));
    listen(listening, SOMAXCONN);

    client_sock = accept(listening, (struct sockaddr *) &client, &client_size);

    if (getnameinfo((struct sockaddr *) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        printf("%s connected on port %s\n", host, service);
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        printf("%s connected on port %d\n", host, ntohs(client.sin_port));
    }

    close(listening);
    char buf[4096];
    while (1) {
        memset(buf, 0, 4096);
        int bytes_received = recv(client_sock, buf, 4096, 0);

        if (bytes_received < 0) {
            printf("error in recv() quitting...\n");
            break;
        }
        if (bytes_received == 0) {
            printf("client disconnected...\n");
            break;
        }

        printf("%s", buf);
        // echo msg back to client
        send(client_sock, buf, bytes_received + 1, 0);
    }
    close(client_sock);

    return EXIT_SUCCESS;
}
