//
//  server.c
//  p2p
//
//  Created by Emir Tuncbilek on 12/25/23.
//

#include "./server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void exitWithError(const char * msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

Server serverConstructor(int domain, int service, int protocol, u_long interface, int port, int backlog) {
    Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    // setup server address
    // IPv4
    server.address.sin_family = AF_INET;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);
    // create a socket
    
    // IPv6
    server.addressIPv6.sin6_family = AF_INET6;
    server.addressIPv6.sin6_port = htons(port); // port not used yet
    server.addressIPv6.sin6_addr = in6addr_any;

    // create a socket
    server.socket = socket(domain, service, protocol);
    
    if ( server.socket < 0 ) {
        
        exitWithError("\nCouldn't create socket :(\n");
    }

    // choose the right address depending on the protocol
    struct sockaddr * address = domain == AF_INET ? (struct sockaddr *) &server.address : (struct sockaddr *) &server.addressIPv6;
    
    socklen_t addressSize = (domain == AF_INET) ? sizeof(server.address) : sizeof(server.addressIPv6);
        
    // bind socket to network
    if (bind(server.socket, address, addressSize) < 0){
        
        exitWithError("\nCouldn't bind socket :(\n");
    }
    
    server.shouldRun = 1;
    
    printf("Server is all set to go!\n");

    return server;
}

void * serverFunction(void * args) {
    
    ServerArgs * data = (ServerArgs *) args;
    
    printf("Server is serving...\n");
    
    // struct ServerArgs * data = (struct ServerArgs *) args;
    
    char addressBuffer[data->server->domain == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN];
    
    char msg[data->bufferSize];
    
    while (data->server->shouldRun) {
        
        memset(msg, 0, data->bufferSize);
        memset(addressBuffer, 0, data->server->domain == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN);
        
        recvfrom(data->server->socket, msg, data->bufferSize, data->server->protocol, NULL, NULL);
        
        inet_ntop(data->server->domain, &data->server->address.sin_addr, addressBuffer, data->server->domain == AF_INET ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN);

        printf("Message from <%s> : %s\n", addressBuffer, msg);
        
        // thread safe copy
        pthread_mutex_lock(data->mutex);
        strcpy(data->message, msg);
        pthread_mutex_unlock(data->mutex);
        
    }
    
    free(args);
     
    return NULL;
}

ServerArgs serverArgsConstructor(Server * server, int bufferSize, char * message, pthread_mutex_t * mutex) {
    
    ServerArgs args;
    
    args.server = server;
    args.bufferSize = bufferSize;
    args.mutex = mutex;
    args.message = message;
    
    return args;
}

void terminate(Server * server) {
    
    shutdown(server->socket, SHUT_RDWR);
    server->shouldRun = 0;
    close(server->socket);
    
}
