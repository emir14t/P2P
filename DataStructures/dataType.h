//
//  dataType.h
//  p2p
//
//  Created by Emir Tuncbilek on 12/30/23.
//

#ifndef dataType_h
#define dataType_h

#include <stdio.h>
#include <sys/types.h>

typedef struct {
    
    char * IPAddress;
    int portInUse;
    u_long peerID;
    double attributeForCmp;
    int domain;
    int service;
    
} PeerInfo;

PeerInfo peerInfoConstructor(char * IPAddress, int portInUse, u_long peerID, int service, int protocol);

void updatePeerInfoAttribute(PeerInfo * info, double newAttribute);

__attribute__((unused)) int greaterThan(double a, double b);

__attribute__((unused)) int lesserThan(double a, double b);

PeerInfo deepCopy(PeerInfo * other);

int comparePeerInfos(PeerInfo * src, PeerInfo * other, int (*cmpOperation)(double, double));
#endif /* dataType_h */
