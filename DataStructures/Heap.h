//
//  Heap.h
//  p2p
//
//  Created by Emir Tuncbilek on 12/29/23.
//

#ifndef Heap_h
#define Heap_h

#include <stdio.h>

#include "./dataType.h"

#define MAX_INFO 20     // Max number of other peers one can connect to
#define FLOOR_INFO 10   // delete won't work if there are less than FLOOR_INFO peers one is connected to (prevents isolation in the network)

typedef struct {
    
    int currentSize;
    PeerInfo * infos[MAX_INFO];
    int (* cmpOperator)(double, double);  // choose the type of heap you want
    
} Heap ;

Heap heapConstructor(int (* cmpOperator)(double , double ));

__attribute__((unused)) int findElement(Heap * heap, u_long peerID);

__attribute__((unused)) int deleteElement(Heap * heap, u_long peerID);

__attribute__((unused)) static void recursiveDelete(Heap * heap, int elementPos);

int insertElement(Heap * heap, PeerInfo * newInfo);

void percolate(Heap * heap, int posUp, int posDown);

Heap heapDeepCopy(Heap * heap, PeerInfo * buffer);

PeerInfo pop(Heap * heap);

#endif /* Heap_h */
