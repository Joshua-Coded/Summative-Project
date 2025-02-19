// friend_detection.c
#include "friend_detection.h"

// Clear screen function (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayHeader() {
    printf("\033[1;34m"); // Blue bold text
    printf("===================================================\n");
    printf("       FORENSIC FRIEND DETECTION SYSTEM v1.0       \n");
    printf("===================================================\n");
    printf("\033[0m"); // Reset text formatting
}

// Initialize the graph as an empty adjacency matrix
void initializeGraph(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], int* nodeCount) {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph[i][j] = 0;
        }
        contacts[i].phone[0] = '\0';
        contacts[i].name[0] = '\0';
    }
    *nodeCount = 0;
}

// Add a contact to the system
void addContact(Contact contacts[MAX_NODES], int* nodeCount, const char* phone, const char* name) {
    // Check if contact already exists
    for (int i = 0; i < *nodeCount; i++) {
        if (strcmp(contacts[i].phone, phone) == 0) {
            // Update name if provided and different
            if (name[0] != '\0' && strcmp(contacts[i].name, name) != 0) {
                strcpy(contacts[i].name, name);
            }
            return;
        }
    }
    
    // Add new contact
    strcpy(contacts[*nodeCount].phone, phone);
    if (name[0] != '\0') {
        strcpy(contacts[*nodeCount].name, name);
    } else {
        strcpy(contacts[*nodeCount].name, "[Unknown]");
    }
    (*nodeCount)++;
}

// Add an edge between two phone numbers in the graph
void addEdge(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], const char* caller, const char* callee, int* nodeCount) {
    int callerIndex = -1, calleeIndex = -1;

    // Find or add the caller
    for (int i = 0; i < *nodeCount; i++) {
        if (strcmp(contacts[i].phone, caller) == 0) {
            callerIndex = i;
            break;
        }
    }
    if (callerIndex == -1) {
        strcpy(contacts[*nodeCount].phone, caller);
        strcpy(contacts[*nodeCount].name, "[Unknown]");
        callerIndex = *nodeCount;
        (*nodeCount)++;
    }

    // Find or add the callee
    for (int i = 0; i < *nodeCount; i++) {
        if (strcmp(contacts[i].phone, callee) == 0) {
            calleeIndex = i;
            break;
        }
    }
    if (calleeIndex == -1) {
        strcpy(contacts[*nodeCount].phone, callee);
        strcpy(contacts[*nodeCount].name, "[Unknown]");
        calleeIndex = *nodeCount;
        (*nodeCount)++;
    }

    // Add the edge
    graph[callerIndex][calleeIndex] = 1;
    graph[calleeIndex][callerIndex] = 1; // Assuming undirected graph
}

// Find direct contacts of a given phone number
void findDirectContacts(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], int nodeCount, const char* targetNumber) {
    int targetIndex = -1;

    // Find the index of the target number
    for (int i = 0; i < nodeCount; i++) {
        if (strcmp(contacts[i].phone, targetNumber) == 0) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        printf("\n\033[1;31mPHONE NUMBER NOT FOUND\033[0m: %s is not in the database.\n", targetNumber);
        return;
    }

    printf("\n\033[1;32mDIRECT CONTACTS OF %s (%s):\033[0m\n", 
           contacts[targetIndex].phone, 
           contacts[targetIndex].name);
    printf("---------------------------------------------\n");
    
    int contactsFound = 0;
    for (int i = 0; i < nodeCount; i++) {
        if (graph[targetIndex][i] == 1) {
            printf("  [\033[1;36m%d\033[0m] \033[1;33m%-15s\033[0m | %s\n", 
                   ++contactsFound, 
                   contacts[i].phone, 
                   contacts[i].name);
        }
    }
    
    if (contactsFound == 0) {
        printf("  No direct contacts found.\n");
    }
    
    printf("---------------------------------------------\n");
    printf("Total contacts: %d\n\n", contactsFound);
}

// Display the adjacency matrix of the graph
void displayAdjacencyMatrix(int graph[MAX_NODES][MAX_NODES], Contact contacts[MAX_NODES], int nodeCount) {
    printf("\n\033[1;35mNETWORK ADJACENCY MATRIX\033[0m\n");
    printf("===================================================\n\n");
    
    // Print column headers
    printf("       ");
    for (int i = 0; i < nodeCount; i++) {
        printf("%-6d", i);
    }
    printf("\n       ");
    for (int i = 0; i < nodeCount; i++) {
        printf("------");
    }
    printf("\n");
    
    // Print rows with labels
    for (int i = 0; i < nodeCount; i++) {
        printf("[%2d] | ", i);
        for (int j = 0; j < nodeCount; j++) {
            if (graph[i][j] == 1) {
                printf("\033[1;32m%-6d\033[0m", graph[i][j]); // Green for connections
            } else {
                printf("%-6d", graph[i][j]);
            }
        }
        printf("| %s\n", contacts[i].phone);
    }
    
    printf("\n\033[1;33mCONTACT INDEX REFERENCE:\033[0m\n");
    printf("----------------------------\n");
    for (int i = 0; i < nodeCount; i++) {
        printf("  [%2d] %-15s | %s\n", i, contacts[i].phone, contacts[i].name);
    }
    printf("\n");
}

// Display all contacts in the system
void displayAllContacts(Contact contacts[MAX_NODES], int nodeCount) {
    clearScreen();
    displayHeader();
    
    printf("\n\033[1;33mCONTACT DATABASE\033[0m\n");
    printf("===================================================\n");
    printf("  %-4s | %-15s | %s\n", "ID", "PHONE NUMBER", "CONTACT NAME");
    printf("  -----+------------------+-------------------------\n");
    
    for (int i = 0; i < nodeCount; i++) {
        printf("  %-4d | %-15s | %s\n", i+1, contacts[i].phone, contacts[i].name);
    }
    
    printf("===================================================\n");
    printf("Total Contacts: %d\n\n", nodeCount);
    
    printf("Press Enter to return to main menu...");
    getchar(); // Consume newline
    getchar(); // Wait for Enter
}

// Search for a contact by phone number or partial name
void searchContact(Contact contacts[MAX_NODES], int nodeCount) {
    clearScreen();
    displayHeader();
    
    char searchTerm[50];
    printf("\n\033[1;33mCONTACT SEARCH\033[0m\n");
    printf("===================================================\n");
    printf("Enter phone number or name to search: ");
    scanf("%s", searchTerm);
    getchar(); // Consume newline
    
    printf("\n\033[1;36mSEARCH RESULTS FOR \"%s\":\033[0m\n", searchTerm);
    printf("  %-4s | %-15s | %s\n", "ID", "PHONE NUMBER", "CONTACT NAME");
    printf("  -----+------------------+-------------------------\n");
    
    int resultsFound = 0;
    for (int i = 0; i < nodeCount; i++) {
        if (strstr(contacts[i].phone, searchTerm) || 
            strstr(contacts[i].name, searchTerm)) {
            printf("  %-4d | %-15s | %s\n", i+1, contacts[i].phone, contacts[i].name);
            resultsFound++;
        }
    }
    
    if (resultsFound == 0) {
        printf("  No matching contacts found.\n");
    }
    
    printf("===================================================\n");
    printf("Found %d match(es)\n\n", resultsFound);
    
    printf("Press Enter to return to main menu...");
    getchar(); // Wait for Enter
}

// Display main menu
void displayMainMenu() {
    clearScreen();
    displayHeader();
    
    printf("\n\033[1;33mMAIN MENU\033[0m\n");
    printf("===================================================\n");
    printf("  1. Find Direct Contacts for a Phone Number\n");
    printf("  2. Display All Contacts\n");
    printf("  3. Search Contact Database\n");
    printf("  4. Display Network Adjacency Matrix\n");
    printf("  5. Exit\n");
    printf("===================================================\n");
    printf("Enter your choice (1-5): ");
}

// Get menu choice with validation
int getMenuChoice() {
    int choice;
    char buffer[10];
    
    fgets(buffer, sizeof(buffer), stdin);
    if (sscanf(buffer, "%d", &choice) != 1) {
        choice = 0;
    }
    
    return choice;
}
