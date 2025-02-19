#include "friend_detection.h"

int main() {
    int graph[MAX_NODES][MAX_NODES];
    Contact contacts[MAX_NODES];
    int nodeCount = 0;
/*    char buffer[100] ;*/

    // Initialize the graph
    initializeGraph(graph, contacts, &nodeCount);

    // Add contacts with names
    addContact(contacts, &nodeCount, "0781", "John Smith");
    addContact(contacts, &nodeCount, "0782", "Maria Garcia");
    addContact(contacts, &nodeCount, "0783", "David Lee");
    addContact(contacts, &nodeCount, "0784", "Sarah Johnson");
    addContact(contacts, &nodeCount, "0785", "Michael Brown");
    addContact(contacts, &nodeCount, "0786", "Emily Wilson");
    addContact(contacts, &nodeCount, "0787", "Robert Taylor");
    addContact(contacts, &nodeCount, "0788", "Jennifer Davis");

    // Add edges based on the provided data
    addEdge(graph, contacts, "0781", "0782", &nodeCount);
    addEdge(graph, contacts, "0781", "0783", &nodeCount);
    addEdge(graph, contacts, "0782", "0784", &nodeCount);
    addEdge(graph, contacts, "0783", "0785", &nodeCount);
    addEdge(graph, contacts, "0784", "0785", &nodeCount);
    addEdge(graph, contacts, "0784", "0786", &nodeCount);
    addEdge(graph, contacts, "0785", "0787", &nodeCount);
    addEdge(graph, contacts, "0786", "0788", &nodeCount);

    int choice;
    char phoneNumber[15];

    do {
        displayMainMenu();
        choice = getMenuChoice();
        
        switch (choice) {
            case 1:
                clearScreen();
                displayHeader();
                printf("\n\033[1;33mFIND DIRECT CONTACTS\033[0m\n");
                printf("===================================================\n");
                printf("Enter phone number: ");
                scanf("%s", phoneNumber);
                getchar(); // Consume newline
                
                findDirectContacts(graph, contacts, nodeCount, phoneNumber);
                
                printf("Press Enter to return to main menu...");
                getchar();
                break;
                
            case 2:
                displayAllContacts(contacts, nodeCount);
                break;
                
            case 3:
                searchContact(contacts, nodeCount);
                break;
                
            case 4:
                clearScreen();
                displayHeader();
                displayAdjacencyMatrix(graph, contacts, nodeCount);
                
                printf("Press Enter to return to main menu...");
                getchar();
                break;
                
            case 5:
                clearScreen();
                printf("\033[1;34m");
                printf("===================================================\n");
                printf("       THANK YOU FOR USING FRIEND DETECTION        \n");
                printf("===================================================\n");
                printf("\033[0m");
                printf("\nExiting the program...\n\n");
                break;
                
            default:
                printf("\n\033[1;31mInvalid choice. Press Enter and try again.\033[0m");
                getchar();
        }
    } while (choice != 5);

    return 0;
}
