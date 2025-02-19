#include "friend_detection.h"

// Initialize the graph as an empty adjacency matrix
void initializeGraph(int graph[MAX_NODES][MAX_NODES], int* nodeCount) {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph[i][j] = 0;
        }
    }
    *nodeCount = 0;
}

// Add an edge between two phone numbers in the graph
void addEdge(int graph[MAX_NODES][MAX_NODES], const char* caller, const char* callee, int* nodeCount) {
    static char nodes[MAX_NODES][10]; // Store unique phone numbers
    int callerIndex = -1, calleeIndex = -1;

    // Find or add the caller
    for (int i = 0; i < *nodeCount; i++) {
        if (strcmp(nodes[i], caller) == 0) {
            callerIndex = i;
            break;
        }
    }
    if (callerIndex == -1) {
        strcpy(nodes[*nodeCount], caller);
        callerIndex = *nodeCount;
        (*nodeCount)++;
    }

    // Find or add the callee
    for (int i = 0; i < *nodeCount; i++) {
        if (strcmp(nodes[i], callee) == 0) {
            calleeIndex = i;
            break;
        }
    }
    if (calleeIndex == -1) {
        strcpy(nodes[*nodeCount], callee);
        calleeIndex = *nodeCount;
        (*nodeCount)++;
    }

    // Add the edge
    graph[callerIndex][calleeIndex] = 1;
    graph[calleeIndex][callerIndex] = 1; // Assuming undirected graph
}

// Find direct contacts of a given phone number
void findDirectContacts(int graph[MAX_NODES][MAX_NODES], int nodeCount, const char* targetNumber) {
    static char nodes[MAX_NODES][10]; // Store unique phone numbers
    int targetIndex = -1;

    // Find the index of the target number
    for (int i = 0; i < nodeCount; i++) {
        if (strcmp(nodes[i], targetNumber) == 0) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        printf("Phone number %s not found in the graph.\n", targetNumber);
        return;
    }

    printf("Direct contacts of %s:\n", targetNumber);
    for (int i = 0; i < nodeCount; i++) {
        if (graph[targetIndex][i] == 1) {
            printf("- %s\n", nodes[i]);
        }
    }
}

// Display the adjacency matrix of the graph
void displayAdjacencyMatrix(int graph[MAX_NODES][MAX_NODES], int nodeCount) {
    static char nodes[MAX_NODES][10]; // Store unique phone numbers

    printf("\nAdjacency Matrix:\n");
    printf("   ");
    for (int i = 0; i < nodeCount; i++) {
        printf("%-5s", nodes[i]);
    }
    printf("\n");

    for (int i = 0; i < nodeCount; i++) {
        printf("%-3s", nodes[i]);
        for (int j = 0; j < nodeCount; j++) {
            printf("%-5d", graph[i][j]);
        }
        printf("\n");
    }
}
