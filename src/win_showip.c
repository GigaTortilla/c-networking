#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>
#include <ws2ipdef.h>
#include <ws2def.h>
#include <WS2tcpip.h>
#include <WSPiApi.h>

int main(int argc, char **argv) {
    char *port = "http";

    if (2 > argc || 3 < argc) {
        fputs("invalid argument number\nusage: showip hostname [port]\n", stdout);
        return EXIT_FAILURE;
    } else if (3 == argc) port = argv[2];
    
    char *host_name = argv[1];
    char ipstr[INET6_ADDRSTRLEN];
    int status, wsa_status;
    struct addrinfo hints, *host_info, *p_iter;
    WSADATA wsa_data;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    // Call WSAStartup to initialize the Windows Sockets DLLs with version 2.2
    if ((wsa_status = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
        fprintf(stdout, "WSAStartup failed: %s\n", wsa_status);
        return EXIT_FAILURE;
    }

    // Get the infos for the specified address store infos in struct host_info
    if ((status = getaddrinfo(host_name, port, &hints, &(host_info))) != 0) {
        fprintf(stdout, "getaddrinfo error: %s\n", gai_strerror(status));
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("IP addresses for %s:\n\n", host_name);

    for (p_iter = host_info; p_iter != NULL; p_iter = p_iter->ai_next) {
        void *addr;
        char *ipver;

        if (p_iter->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p_iter->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p_iter->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p_iter->ai_family, addr, ipstr, sizeof(ipstr));
        printf(" %s: %s\n", ipver, ipstr);
    }
    WSACleanup();
    return EXIT_SUCCESS;
}