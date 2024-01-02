//
//  client.c
//  p2p
//
//  Created by Emir Tuncbilek on 12/26/23.
//

#include "client.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
// #include <pthread.h>


Client clientConstructor(int domain, int service, int protocol, int port) {
    
    Client client;
    
    client.domain = domain;
    client.service = service;
    client.protocol = protocol;
    client.port = port;
        
    return client;
}

requestArgs requestArgsConstructor (Client * client, char * serverIP, char * message, int requestBuffer) {
    
    requestArgs args;
    
    args.client = client;
    args.serverIP = serverIP;
    args.message = message;
    args.requestBuffer = requestBuffer;
    
    return args;
}

void * request(void * args) {
    
    requestArgs * data = (requestArgs *) args;
    
    // create server address
    struct sockaddr_in serverAddressIPv4;
    struct sockaddr_in6 serverAddressIPv6;
    
    // IPv4
    serverAddressIPv4.sin_family = AF_INET;
    serverAddressIPv4.sin_port = htons(data->client->port);
    inet_aton(data->serverIP, &serverAddressIPv4.sin_addr);
    
    // IPv6
    serverAddressIPv6.sin6_family = AF_INET6;
    serverAddressIPv6.sin6_port = htons(data->client->port);
    inet_pton(AF_INET6, data->serverIP, &serverAddressIPv6.sin6_addr);
    
    data->client->socket = socket(data->client->domain, data->client->service, data->client->protocol);
    
    struct sockaddr* serverAddress = (data->client->domain == AF_INET) ? (struct sockaddr *) &serverAddressIPv4 : (struct sockaddr *) &serverAddressIPv6;
    
    sendto(data->client->socket, data->message, data->requestBuffer, data->client->protocol, serverAddress, (data->client->domain == AF_INET) ? sizeof(serverAddressIPv4) : sizeof(serverAddressIPv6));
    
    printf("Message Sent to IP: %s\n", data->serverIP);
    
    free(args);
    
    return NULL;
}
