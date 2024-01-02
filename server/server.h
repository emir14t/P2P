//
//  server.h
//  p2p
//
//  Created by Emir Tuncbilek on 12/25/23.
//

#ifndef server_h
#define server_h

#include <sys/types.h>
#include <netinet/in.h>


typedef struct {
    /* Server public variables */
    
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;
    // IPv4
    struct sockaddr_in address;
    // IPv6
    struct sockaddr_in6 addressIPv6;
    
    short shouldRun;
    int socket;
} Server ;

Server serverConstructor(int domain, int service, int protocol, u_long interface, int port, int backlog);

typedef struct {
    Server * server;
    // char * message;
    int bufferSize;
    pthread_mutex_t * mutex;
    char * message;
} ServerArgs ;

ServerArgs serverArgsConstructor(Server * server, int bufferSize, char * message, pthread_mutex_t * mutex);

void * serverFunction(void * args);

void terminate(Server * server);

#endif
