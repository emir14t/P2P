//
//  client.h
//  p2p
//
//  Created by Emir Tuncbilek on 12/26/23.
//

#ifndef client_h
#define client_h

#include <stdio.h>
#include <sys/types.h>

typedef struct {
    /* Client public variables */
    
    int socket;
    int domain;
    int service;
    int protocol;
    int port;
    
} Client;

Client clientConstructor(int domain, int service, int protocol, int port);

typedef struct {
    Client * client;
    char * serverIP;
    char* message;
    int requestBuffer;
    pthread_cond_t * conditionSent;
} requestArgs;

requestArgs requestArgsConstructor (Client * client, char * serverIP, char* message, int requestBuffer);

void * request(void * args);

#endif /* client_h */
