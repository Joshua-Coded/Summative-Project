#ifndef FRIEND_DETECTION_H
#define FRIEND_DETECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the maximum number of nodes
#define MAX_NODES 100
#define MAX_NAME_LENGTH 50

typedef struct {
    char phone[15];
    char name[MAX_NAME_LENGTH];
} Contact;

// Function prototypes
void initializeGraph(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], int* nodeCount);
void addContact(Contact contacts[MAX_NODES], int* nodeCount, const char* phone, const char* name);
void addEdge(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], const char* caller, const char* callee, int* nodeCount);
void findDirectContacts(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], int nodeCount, const char* targetNumber);
void displayAdjacencyMatrix(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], int nodeCount);
void displayAllContacts(Contact contacts[MAX_NODES], int nodeCount);
void clearScreen();
void displayMainMenu();
int getMenuChoice();
void searchContact(Contact contacts[MAX_NODES], int nodeCount);
void displayHeader();

#endif // FRIEND_DETECTION_H
