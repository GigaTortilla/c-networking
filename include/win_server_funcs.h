#ifndef WIN_SERVER_FUNCS_H
#define WIN_SERVER_FUNCS_H

// specifies the port a client has to connect to
#define PORT "54590"
// number of possible client connections to the server
#define BACKLOG 5

// get the socket address, IPv4 or IPv6 respectively
void *get_in_addr(struct sockaddr *sa);

#endif