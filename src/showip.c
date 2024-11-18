#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: showip HOSTNAME");
        return EXIT_FAILURE;
    }

    int status;
    struct addrinfo hints, *serv_info, *ai_ptr;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &serv_info)) != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }
    printf("IP address(es) for %s\n\n", argv[1]);
    for (ai_ptr = serv_info; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
        void *addr;
        char *ipver;

        if (ai_ptr->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) ai_ptr->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) ai_ptr->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        inet_ntop(ai_ptr->ai_family, addr, ipstr, sizeof(ipstr));
        printf(" %s: %s\n", ipver, ipstr);
    }

    freeaddrinfo(serv_info);
    return EXIT_SUCCESS;
}
