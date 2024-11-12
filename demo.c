//
//  main.c
//  p2p
//
//  Created by Emir Tuncbilek on 12/28/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#include "./Peer/peer.h"

#define PROTOCOL 0
#define IN_ORDER 0

int main (int argc, char * argv[]) {
    
    if (argc < 2 || argc > 4) {
        perror("Unexpected number of arguments, need to specify at least <PORT> : <PORT> <MAX CLIENTS IN LINE> <BUFFER SIZE>\n");
        exit(EXIT_FAILURE);
    }

    const int PORT_SERVER = atoi(argv[1]);
    const int MAX_CLIENTS_IN_LINE = argc > 2 ? atoi(argv[2]) : 1;
    const int BUFFER_SIZE = argc == 4 ? atoi(argv[3]) : 255;


    // for demo purposes :
    const int PORT_CLIENT = PORT_SERVER;    // int PORT_CLIENT is used as a base for generating the port numbers of the other peers

    // ("::1" is a loopback address used for testing purposes)
    char * ips[12] = {"::1", "::2", "::3", "::4", "::5", "::6", "::7", "::8", "::9", "::10", "::11", "::12"};
    PeerInfo peersToSendTo[12];

    srand((u_int) time(NULL));
    for (int i = 0; i < 12; i ++) {

        peersToSendTo[i] = peerInfoConstructor(ips[i], PORT_CLIENT + i, i, AF_INET6, SOCK_DGRAM);
        double randomNum = (double)rand() / RAND_MAX;
        updatePeerInfoAttribute(&peersToSendTo[i], randomNum);
    }
    
    // MARK: initialize
    
    pthread_t peerServerThread;
    pthread_mutex_t messageLock = PTHREAD_MUTEX_INITIALIZER;    // for thread safe purposes (required)
    
    Peer * peer = malloc(sizeof(Peer));
    *peer = peerConstructor(AF_INET6, SOCK_DGRAM, PROTOCOL, INADDR_ANY, PORT_SERVER, MAX_CLIENTS_IN_LINE, BUFFER_SIZE, lesserThan);
    
    char message[BUFFER_SIZE];
    
    // MARK: Listen to incoming messages asynchronously :
    startUpPeerCommunication(peer, message, &peerServerThread, &messageLock);
    
    // MARK: in the mean time, do some shit ...

    // container for sending messages
    messageArgs msg = messageArgsConstructor(peer, NULL, IN_ORDER);

    // add peers to communication heap
    for (int i = 0; i < 12; i++) {
        addPeerToCommunicateTo(peer, &peersToSendTo[i]);
    }
    // remove peer from communication heap
    removePeerFromCommunicationHeap(peer, 5);

    printf("Enter a message (\"EXIT\" stops the server) :\n");
    
    while (strcmp(message, "EXIT\n") != 0) {
        
        // loop does not necessarily exit when "EXIT" is sent due to network latency
        
        char request[BUFFER_SIZE];
        memset(request, 0, BUFFER_SIZE);
        fgets(request, BUFFER_SIZE, stdin);

        // update the message container with what you want to send to the other peers
        updateMessage(&msg, request);
        
        // MARK: send message to other peers
        // there is a possibility to make this async, but it does not apply here, because we are already listening to messages asynchronously
        sendPeersMessage((void *) &msg);
    }
    
    // MARK: at the end of the exchange ...    (required for no memory leaks)

    terminatePeerConnection(peer);
    pthread_join(peerServerThread, NULL);
    pthread_mutex_destroy(&messageLock);
    
    printf("Finito\n");
}
