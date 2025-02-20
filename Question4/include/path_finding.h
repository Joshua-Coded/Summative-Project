#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Define the maximum number of cities
#define MAX_CITIES 100

// Function prototypes
void initializeGraph(int graph[MAX_CITIES][MAX_CITIES], int* cityCount);
void addEdge(int graph[MAX_CITIES][MAX_CITIES], int* cityCount, const char* from, const char* to, int time);
void dijkstra(int graph[MAX_CITIES][MAX_CITIES], int cityCount, const char* startCity, const char* endCity);
void displayShortestPath(const char cities[][50], int distances[MAX_CITIES], 
                        int previousNodes[MAX_CITIES], int startCityIndex, int endCityIndex);
void displayAvailableCities(int cityCount);
int findCityIndex(int cityCount, const char* cityName);
void normalizeCity(const char* input, char* output);

#endif // PATH_FINDING_H
