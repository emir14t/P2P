//
//  dataType.c
//  p2p
//
//  Created by Emir Tuncbilek on 12/30/23.
//

#include "dataType.h"

PeerInfo peerInfoConstructor(char * IPAddress, int portInUse, u_long peerID, int domain, int service) {
    
    PeerInfo info;
    
    info.IPAddress = IPAddress;
    info.portInUse = portInUse;
    info.peerID = peerID;
    info.attributeForCmp = 0.;
    info.domain = domain;
    info.service = service;
    
    return info;
}

void updatePeerInfoAttribute(PeerInfo * info, double newAttribute) {
    info->attributeForCmp = newAttribute;
}

__attribute__((unused)) int greaterThan(double a, double b) {
    if (a > b)
        return 1;
    if (a == b)
        return 0;
    return -1;
}

__attribute__((unused)) int lesserThan(double a, double b) {
    if (a < b)
        return 1;
    if (a == b)
        return 0;
    return -1;
}

PeerInfo deepCopy(PeerInfo * other) {
    PeerInfo info = peerInfoConstructor(other->IPAddress, other->portInUse, other->peerID, other->domain, other->service);
    
    info.attributeForCmp = other->attributeForCmp;
    
    return info;
}

int comparePeerInfos(PeerInfo * src, PeerInfo * other, int (*cmpOperation)(double, double)) {
    
    int cmpOp = cmpOperation(src->attributeForCmp, other->attributeForCmp);
    
    return cmpOp;
}

