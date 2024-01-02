//
//  Heap.c
//  p2p
//
//  Created by Emir Tuncbilek on 12/29/23.
//

#include "Heap.h"

#include <stdlib.h>

Heap heapConstructor(int (* cmpOperator)(double , double )) {
    Heap heap;
    
    heap.currentSize = 1;
    heap.cmpOperator = cmpOperator;
    heap.infos[0] = NULL;

    return heap;
}

int findElement(Heap * heap, u_long peerID) {
    int i;
    for (i = 1; i < heap->currentSize; i ++) {
        if (heap->infos[i]->peerID == peerID) {
            return i;
        }
    }
    return 0;
}

int deleteElement(Heap * heap, u_long peerID) {
    
    int elementPos = findElement(heap, peerID);
    
    if (elementPos == 0)
        return 0;
    
    recursiveDelete(heap, elementPos);
    heap->currentSize --;
    return 1;
}

void recursiveDelete(Heap * heap, int elementPos) {
    
    int leftChild = elementPos * 2;
    int rightChild = (elementPos * 2) + 1;
    int noLeftChild = 0;
    
    if (leftChild >= (heap->currentSize - 1) &&  rightChild >= (heap->currentSize - 1)) {
        // the node has no children
        // just delete the element by shifting leaf nodes up
        for (int i = elementPos; i < heap->currentSize - 1; i++) {
            heap->infos[i] = heap->infos[i + 1];
        }
        heap->infos[heap->currentSize - 1] = NULL;
        return;
    }
    
    if (leftChild < (heap->currentSize - 1) && rightChild < (heap->currentSize - 1)) {
        // the node has both children
        // choose the favorite child to replace the node
        int cmpOp = comparePeerInfos(heap->infos[leftChild], heap->infos[rightChild], heap->cmpOperator);
        
        noLeftChild = cmpOp == 1 ? 0 : 1;
        
    }
    
    if (leftChild >= (heap->currentSize - 1) || noLeftChild == 1) {
        // the node has no left child so go right
        *heap->infos[elementPos] = *heap->infos[rightChild];
        recursiveDelete(heap, rightChild);
        
    } else {
        // the node has no right child so go left
        *heap->infos[elementPos] = *heap->infos[leftChild];
        recursiveDelete(heap, leftChild);
    }
}


int insertElement(Heap * heap, PeerInfo * newInfo) {
    
    if (heap->currentSize == 1) {
        heap->infos[heap->currentSize ++] = newInfo;
        return 1;
    }
    
    if (heap->currentSize == MAX_INFO)
        return 0;
    
    heap->infos[heap->currentSize] = newInfo;
    
    percolate(heap, heap->currentSize % 2 == 0 ? heap->currentSize / 2 : (heap->currentSize - 1) / 2, heap->currentSize);
    
    heap->currentSize ++;
    
    return 1;
}

void percolate(Heap * heap, int posUp, int posDown) {
    if (posUp == 0)
        return;
    
    // out of order
    if (comparePeerInfos(heap->infos[posUp], heap->infos[posDown], heap->cmpOperator) == -1) {
        // invert
        PeerInfo temp = *heap->infos[posUp];
        
        *heap->infos[posUp]= *heap->infos[posDown];
        
        *heap->infos[posDown] = temp;
        
        percolate(heap, posUp % 2 == 0 ? posUp / 2 : (posUp - 1) / 2, posUp);
    }
    return;
}

Heap heapDeepCopy(Heap * heap, PeerInfo * buffer) {
    Heap heapCopy = heapConstructor(heap->cmpOperator);
    
    for (int i = 1; i < heap->currentSize; i++) {
        buffer[i - 1] = deepCopy(heap->infos[i]);
        insertElement(&heapCopy, &buffer[i - 1]);
    }
    
    return heapCopy;
}

PeerInfo pop(Heap * heap) {
    PeerInfo info;
    if (heap->currentSize > 1) {
        info = deepCopy(heap->infos[1]);
        recursiveDelete(heap, 1);
        heap->currentSize --;

    } else {
        perror("Can't pop() an empty heap\n");
        info = peerInfoConstructor("0", -1, 0, -1, -1);
    }
   
    return info;
}
