#include "path_finding.h"
#include <ctype.h>

int main() {
    int graph[MAX_CITIES][MAX_CITIES];
    int cityCount = 0;
    
    // Initialize the graph
    initializeGraph(graph, &cityCount);
    
    // Add edges based on the provided data
    addEdge(graph, &cityCount, "Bumbogo", "Nayinzira", 10);
    addEdge(graph, &cityCount, "Bumbogo", "Kanombe", 30);
    addEdge(graph, &cityCount, "Nayinzira", "Mushimire", 10);
    addEdge(graph, &cityCount, "Mushimire", "Airport", 15);
    addEdge(graph, &cityCount, "Kanombe", "Airport", 5);
    addEdge(graph, &cityCount, "Mushimire", "Kimironko", 3);
    addEdge(graph, &cityCount, "Kimironko", "Remera", 6);
    addEdge(graph, &cityCount, "Remera", "Airport", 4);
    
    printf("\n=============================================\n");
    printf("       CITY PATH FINDING APPLICATION        \n");
    printf("=============================================\n");
    
    while (1) {
        printf("\nOptions:\n");
        printf("1. Find shortest path between cities\n");
        printf("2. View all available cities\n");
        printf("3. Exit\n");
        printf("Choose an option (1-3): ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            // Clear input buffer in case of invalid input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        // Clear the newline from the input buffer
        getchar();
        
        switch (choice) {
            case 1: {
                char startCity[50], endCity[50];
                
                printf("\nEnter start city: ");
                fgets(startCity, sizeof(startCity), stdin);
                startCity[strcspn(startCity, "\n")] = 0;  // Remove trailing newline
                
                printf("Enter destination city: ");
                fgets(endCity, sizeof(endCity), stdin);
                endCity[strcspn(endCity, "\n")] = 0;  // Remove trailing newline
                
                dijkstra(graph, cityCount, startCity, endCity);
                break;
            }
            case 2:
                displayAvailableCities(cityCount);
                break;
            case 3:
                printf("\nThank you for using the Path Finding Application.\n");
                return 0;
            default:
                printf("Invalid option. Please choose 1, 2, or 3.\n");
                break;
        }
    }
    
    return 0;
}
