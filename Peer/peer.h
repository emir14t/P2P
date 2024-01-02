//
//  peer.h
//  p2p
//
//  Created by Emir Tuncbilek on 12/26/23.
//

#ifndef peer_h
#define peer_h

#include <stdio.h>
#include "../DataStructures/Heap.h"
#include "../client/client.h"
#include "../server/server.h"

typedef struct {
    /* Peer public attributs */

    Server * server;
    Heap * peersToSendTo;
    int bufferSize;
    
} Peer;

Peer peerConstructor(int domain, int service, int protocol, u_short interface, int serverPort, int backlog, int bufferSize, int (*f)(double, double));

void startUpPeerCommunication(Peer * peer, char * message, pthread_t * thread, pthread_mutex_t * mutex);

typedef struct {
    Peer * peer;
    char * message;
    int inOrder;
} messageArgs;

messageArgs messageArgsConstructor(Peer * peer, char * message, int inOrder);

void addPeerToCommunicateTo(Peer * peer, PeerInfo * info);

void removePeerFromCommunicationHeap(Peer * peer, u_long peerID);

void updateMessage(messageArgs * args, char * newMessage);

void * sendPeersMessage(void * args);

void terminatePeerConnection(Peer * peer);

#endif /* peer_h */
