//
//  peer.c
//  p2p
//
//  Created by Emir Tuncbilek on 12/26/23.
//

#include "peer.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


Peer peerConstructor(int domain, int service, int protocol, u_short interface, int serverPort, int backlog, int bufferSize, int (*f)(double, double)) {
    
    Peer peer;
        
    peer.server = malloc(sizeof(Server));
    *peer.server = serverConstructor(domain, service, protocol, interface, serverPort, backlog);
    
    peer.peersToSendTo = malloc(sizeof(Heap));
    *peer.peersToSendTo = heapConstructor(f == NULL ? lesserThan : f);
    
    peer.bufferSize = bufferSize;

    return peer;
}


void startUpPeerCommunication(Peer * peer, char * message, pthread_t * thread, pthread_mutex_t * mutex) {
    
    ServerArgs _args = serverArgsConstructor(peer->server, peer->bufferSize, message, mutex);
    
    ServerArgs * args = malloc(sizeof(ServerArgs));
    *args = _args;
    
    pthread_create(thread, NULL, serverFunction, (void *) args);
}

messageArgs messageArgsConstructor(Peer * peer, char * message, int inOrder) {
    
    messageArgs args;
    
    args.peer = peer;
    args.message = message;
    args.inOrder = inOrder;
    
    return args;
}

void addPeerToCommunicateTo(Peer * peer, PeerInfo * info) {
    int elementInserted = insertElement(peer->peersToSendTo, info);
    if (!elementInserted)
        perror("Can't communicate with peer\n");

}

void removePeerFromCommunicationHeap(Peer * peer, u_long peerID) {
    int delStatus = deleteElement(peer->peersToSendTo, peerID);
    if (!delStatus)
        perror("Peer not found :(");
}


void updateMessage(messageArgs * args, char * newMessage) {
    if (args->peer->bufferSize >= sizeof(newMessage))
        args->message = newMessage;
    else
        perror("Message is too long\n");
}

void * sendPeersMessage(void * args) {
    
    messageArgs * data = (messageArgs *) args;
    
    Heap * tmpHeap = malloc(sizeof(Heap));
    
    PeerInfo buffer[data->peer->peersToSendTo->currentSize - 1];
    
    if (data->inOrder == 1)
        *tmpHeap = heapDeepCopy(data->peer->peersToSendTo, buffer);
    

    for (int i = 1; i < data->peer->peersToSendTo->currentSize; i++) {
        
        PeerInfo info = data->inOrder == 1 ? pop(tmpHeap) : *data->peer->peersToSendTo->infos[i] ;
                
        requestArgs * req = malloc(sizeof(requestArgs));
        
        Client tmpClient = clientConstructor(
                info.domain,
                info.service,
                data->peer->server->protocol,
                info.portInUse);
        
        *req = requestArgsConstructor(&tmpClient, info.IPAddress, data->message, data->peer->bufferSize);

        request(req);
    }
        
    free(tmpHeap);
    return NULL;
}

void terminatePeerConnection(Peer * peer) {
    terminate(peer->server);
    free(peer->server);
    free(peer);
}
