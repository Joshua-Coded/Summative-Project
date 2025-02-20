#include "path_finding.h"
#include <ctype.h>

// Declare a global array to store city names
static char cityNames[MAX_CITIES][50];

// Helper function to convert string to lowercase
void toLowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Helper function to normalize city name (trim spaces & lowercase)
void normalizeCity(const char* input, char* output) {
    int i = 0, j = 0;
    
    // Skip leading spaces
    while (isspace(input[i])) i++;
    
    // Copy and convert to lowercase
    while (input[i]) {
        output[j++] = tolower(input[i++]);
    }
    
    // Remove trailing spaces
    while (j > 0 && isspace(output[j-1])) j--;
    
    output[j] = '\0';
}

// Helper function to display all available cities
void displayAvailableCities(int cityCount) {
    printf("\nAvailable cities:\n");
    printf("----------------\n");
    for (int i = 0; i < cityCount; i++) {
        printf("%s\n", cityNames[i]);
    }
    printf("\n");
}

// Initialize the graph as an empty adjacency matrix
void initializeGraph(int graph[MAX_CITIES][MAX_CITIES], int* cityCount) {
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            graph[i][j] = INT_MAX; // Use INT_MAX to represent no direct connection
        }
        graph[i][i] = 0; // Set diagonal elements to 0
    }
    *cityCount = 0;
}

// Add an edge between two cities with a given travel time
void addEdge(int graph[MAX_CITIES][MAX_CITIES], int* cityCount, const char* from, const char* to, int time) {
    int fromIndex = -1, toIndex = -1;

    // Find or add the 'from' city
    for (int i = 0; i < *cityCount; i++) {
        char normalizedCity[50];
        char normalizedFrom[50];
        
        normalizeCity(cityNames[i], normalizedCity);
        normalizeCity(from, normalizedFrom);
        
        if (strcmp(normalizedCity, normalizedFrom) == 0) {
            fromIndex = i;
            break;
        }
    }
    if (fromIndex == -1) {
        strcpy(cityNames[*cityCount], from);
        fromIndex = *cityCount;
        (*cityCount)++;
    }

    // Find or add the 'to' city
    for (int i = 0; i < *cityCount; i++) {
        char normalizedCity[50];
        char normalizedTo[50];
        
        normalizeCity(cityNames[i], normalizedCity);
        normalizeCity(to, normalizedTo);
        
        if (strcmp(normalizedCity, normalizedTo) == 0) {
            toIndex = i;
            break;
        }
    }
    if (toIndex == -1) {
        strcpy(cityNames[*cityCount], to);
        toIndex = *cityCount;
        (*cityCount)++;
    }

    // Add the edge with the given travel time
    graph[fromIndex][toIndex] = time;
    graph[toIndex][fromIndex] = time; // Assuming undirected graph
}

// Find city index with case-insensitive matching
int findCityIndex(int cityCount, const char* cityName) {
    char normalizedInput[50];
    normalizeCity(cityName, normalizedInput);
    
    for (int i = 0; i < cityCount; i++) {
        char normalizedCity[50];
        normalizeCity(cityNames[i], normalizedCity);
        
        if (strcmp(normalizedCity, normalizedInput) == 0) {
            return i;
        }
    }
    return -1;
}

// Dijkstra's algorithm to find the shortest path
void dijkstra(int graph[MAX_CITIES][MAX_CITIES], int cityCount, const char* startCity, const char* endCity) {
    int startCityIndex = findCityIndex(cityCount, startCity);
    int endCityIndex = findCityIndex(cityCount, endCity);

    if (startCityIndex == -1 || endCityIndex == -1) {
        printf("Error: ");
        if (startCityIndex == -1) {
            printf("'%s' not found in the network", startCity);
        }
        if (endCityIndex == -1) {
            if (startCityIndex == -1) printf(" and ");
            printf("'%s' not found in the network", endCity);
        }
        printf(".\n");
        displayAvailableCities(cityCount);
        return;
    }

    // Initialize distances and visited array
    int distances[cityCount];
    int visited[cityCount];
    int previousNodes[cityCount];

    for (int i = 0; i < cityCount; i++) {
        distances[i] = INT_MAX;
        previousNodes[i] = -1;
        visited[i] = 0;
    }
    distances[startCityIndex] = 0;

    // Dijkstra's algorithm
    for (int i = 0; i < cityCount; i++) {
        int minDistance = INT_MAX, minIndex = -1;

        // Find the unvisited node with the smallest distance
        for (int j = 0; j < cityCount; j++) {
            if (!visited[j] && distances[j] < minDistance) {
                minDistance = distances[j];
                minIndex = j;
            }
        }

        if (minIndex == -1) break; // No reachable nodes left

        visited[minIndex] = 1;

        // Update distances to adjacent nodes
        for (int j = 0; j < cityCount; j++) {
            if (!visited[j] && graph[minIndex][j] != INT_MAX &&
                distances[minIndex] + graph[minIndex][j] < distances[j]) {
                distances[j] = distances[minIndex] + graph[minIndex][j];
                previousNodes[j] = minIndex;
            }
        }
    }

    // Display the shortest path
    if (distances[endCityIndex] == INT_MAX) {
        printf("No path exists between %s and %s.\n", cityNames[startCityIndex], cityNames[endCityIndex]);
    } else {
        printf("\nShortest path from %s to %s:\n", cityNames[startCityIndex], cityNames[endCityIndex]);
        printf("------------------------------\n");
        displayShortestPath(cityNames, distances, previousNodes, startCityIndex, endCityIndex);
        printf("\nTotal travel time: %d minutes\n", distances[endCityIndex]);
    }
}

// Display the shortest path
void displayShortestPath(const char cities[][50], int distances[MAX_CITIES], 
                        int previousNodes[MAX_CITIES], int startCityIndex, int endCityIndex) {
    if (endCityIndex == startCityIndex) {
        printf("%s", cities[startCityIndex]);
        return;
    }
    if (previousNodes[endCityIndex] == -1) {
        printf("No path exists.\n");
        return;
    }
    displayShortestPath(cities, distances, previousNodes, startCityIndex, previousNodes[endCityIndex]);
    printf(" -> %s (%d min)", cities[endCityIndex], 
           distances[endCityIndex] - distances[previousNodes[endCityIndex]]);
}
