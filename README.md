# P2P Library for C

A simple P2P communication structure

## Table of Contents

- [Project Name](#project-name)
    - [Table of Contents](#table-of-contents)
    - [Features](#features)
        - [Creating a Peer](#creating-a-peer)
        - [Listening to incoming requests asynchronously](#listening-to-incoming-requests-asynchronously)
        - [Communicating with other peers](#communicating-with-other-peers)
        - [Adding peers to communication heap](#adding-peers-to-communication-heap)
    - [Demo code](#demo-code)
## Features

### Creating a Peer
Creating a Peer structure to start communications:

```c
Peer peerConstructor(int domain, int service, int protocol, u_short interface, int portServer, int backlog, int bufferSize, int (*f)(double, double));
```

Example creating a peer that communicates on IPv6 standard with the UDP protocol, accepting at most 1 request at a time on port 8080 with a maximum message length of 255 characters:
```c
#define PROTOCOL 0
#define PORT_SERVER 8080
#define MAX_CLIENTS_IN_LINE 1
#define BUFFER_SIZE 255

Peer * peer = malloc(sizeof(Peer));
*peer = peerConstructor(AF_INET6, SOCK_DGRAM, PROTOCOL, INADDR_ANY, PORT_SERVER, MAX_CLIENTS_IN_LINE, BUFFER_SIZE, NULL);
```

Note: the ```int (*f)(double double)``` argument is used to sort out priorities on the heap for communicating with other peers and is defaulted to ```lesserThan```. You are more than welcome to use custom functors to establish communication priorities.

### Listening to incoming requests asynchronously
Start up the client server:

```c
void startUpPeerCommunication(Peer * peer, char * message, pthread_t * thread, pthread_mutex_t * mutex);
```

Example with the code from the section above:

```c
/*
 * Code to initialize peer
 * */

pthread_t peerServerThread;
pthread_mutex_t messageLock = PTHREAD_MUTEX_INITIALIZER;
char message[BUFFER_SIZE];
startUpPeerCommunication(peer, message, &peerServerThread, &messageLock);

/*
 * the rest of the code
 * */
```

In order to terminate the listening of the peer server to outside requests, you just need to execute this function:
```c
void terminatePeerConnection(Peer * peer);
```
for good mesures, make sure to join the server thread that you have created at the end of its service and to destroy the thread mutex variable used by the server.

### Communicating with other peers

To send a message to all the other peers you are connected to, the following can be executed:
```c
void * sendPeersMessage(void * args);
```

The reason this method returns a void pointer and also takes in parameter a void pointer is to be able to be used asynchronously with threads, in the event that sending your message becomes blocking code. the parameter args is dynamically casted to be of type ```messageArgs```.

The ```messageArgs``` structure can be created with its constructor. Also, to reuse the same variable for better memory management, the function ```updateMessage``` can be used.
They are respectively defined as follows:

Constructor:
```c
messageArgs messageArgsConstructor(Peer * peer, char * message, int inOrder);
```

Note: the ``inOrder`` parameter is a boolean that specifies whether you want to send the messages following a strict ordering or not. If the parameter is set to 1, the peer will send the message in order of priority with a time complexity of O(n*logn) and a space complexity of O(n). Otherwise, it will send in order in the heap, which could be approximately in order, with a time and space complexity of O(n).

```updateMessage```
```c
void updateMessage(messageArgs * args, char * newMessage);
```

Here is a recap example:
```c
#define IN_ORDER 1

/*
 * Code to initialize peer
 * */

char * newMessage = "Hello World!\0";
messageArgs msg = messageArgsConstructor(peer, NULL, IN_ORDER);
updateMessage(&msg, newMessage);
sendPeersMessage((void *) &msg);
```

### Adding peers to communication heap

Other peer infos are stored in a heap data structure, which allows the peer object to prioritize who it's going to send its message to first. The following functions are defined for adding and removing peer information.
Note : when ```sendPeersMessage``` is called, it automatically handles the priorities for us.

Add to the heap:
```c
void addPeerToCommunicateTo(Peer * peer, PeerInfo * info);
```

Remove from the heap:
```c
void removePeerFromCommunicationHeap(Peer * peer, u_long peerID);
```

the ```PeerInfo``` structure can be constructed with its very own constructor:
```c
PeerInfo peerInfoConstructor(char * IPAddress, int portInUse, u_long peerID, int service, int protocol);
```

Initially, all ```PeerInfo``` structures have the same priority, but one can be set using the following function:
```c
void updatePeerInfoAttribute(PeerInfo * info, double newAttribute);
```

Here is a recap example of the functionalities from this section:
```c
/*
 * Code to initialize peer
 * */

const int PORT_CLIENT = 8080;
char * ips[12] = {"::1", "::2", "::3", "::4", "::5", "::6", "::7", "::8", "::9", "::10", "::11", "::12"};
PeerInfo peersToSendTo[12];

// Generates random priorities and assigns them to each PeerInfo of the array 
srand((u_int) time(NULL));
for (int i = 0; i < 12; i ++) {
    // constructor
    peersToSendTo[i] = peerInfoConstructor(ips[i], PORT_CLIENT + i, i, AF_INET6, SOCK_DGRAM);
    double randomNum = (double)rand() / RAND_MAX;
    // update
    updatePeerInfoAttribute(&peersToSendTo[i], randomNum);
}
for (int i = 0; i < 12; i++) {
    // add to communication heap
    addPeerToCommunicateTo(peer, &peersToSendTo[i]);
}
// remove from communication heap
removePeerFromCommunicationHeap(peer, 5);

/*
 * rest of the code
 * */
```
## Demo code

For a full recap of all the functionalities, visit the ```demo.c``` file at the root of this directory. It's basically a group chat in P2P architecture. A Makefile exists in case you want to test it for yourself. Just run the following commands in your terminal:
```bash
    make
```

```bash
    ./demo <PORT OF SERVER> <MAX CLIENTS> <BUFFER SIZE>
```
Note: Only the first argument (PORT OF SERVER) needs to be specified. the second and third arguments are defaulted to 1 and 255 respectively
# P2P
