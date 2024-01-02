#
#	Makefile for building demo and shared library
#
#	Created by Emir Tuncbilek on 12/31/23.


$(CC) = gcc
CFLAGS = -Wall -g

SERVER_OBJ = server.o
CLIENT_OBJ = client.o
PEER_OBJ = peer.o
DATA_TYPE_OBJ = dataType.o
HEAP_OBJ = heap.o

OBJ_FILES = $(SERVER_OBJ) $(CLIENT_OBJ) $(PEER_OBJ) $(DATA_TYPE_OBJ) $(HEAP_OBJ)

P2P_SHARED_LIB = libP2P.so

BINS = $(P2P_SHARED_LIB) demo

all: $(BINS)

lib: $(P2P_SHARED_LIB)

demo: demo.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lP2P

$(SERVER_OBJ): ./server/server.c ./server/server.h
	$(CC) $(CFLAGS) -c ./server/server.c

$(CLIENT_OBJ): ./client/client.c ./client/client.h
	$(CC) $(CFLAGS) -c ./client/client.c

$(PEER_OBJ): ./Peer/peer.c ./Peer/peer.h
	$(CC) $(CFLAGS) -c ./Peer/peer.c

$(DATA_TYPE_OBJ): ./DataStructures/dataType.c ./DataStructures/dataType.h
	$(CC) $(CFLAGS) -c ./DataStructures/dataType.c

$(HEAP_OBJ): ./DataStructures/Heap.c ./DataStructures/Heap.h
	$(CC) $(CFLAGS) -c ./DataStructures/Heap.c

$(P2P_SHARED_LIB): $(OBJ_FILES)
	$(CC) $(CFLAGS) -fpic -shared -o $@ $(OBJ_FILES) -lc

clean:
	rm *.o

clean_lib:
	rm *.o *.so

clean_all:
	rm *.o *.so demo
