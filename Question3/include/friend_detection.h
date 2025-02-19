#ifndef FRIEND_DETECTION_H
#define FRIEND_DETECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the maximum number of nodes
#define MAX_NODES 100

// Function prototypes
void initializeGraph(int graph[MAX_NODES][MAX_NODES], int* nodeCount);
void addEdge(int graph[MAX_NODES][MAX_NODES], const char* caller, const char* callee, int* nodeCount);
void findDirectContacts(int graph[MAX_NODES][MAX_NODES], int nodeCount, const char* targetNumber);
void displayAdjacencyMatrix(int graph[MAX_NODES][MAX_NODES], int nodeCount);

#endif // FRIEND_DETECTION_H
