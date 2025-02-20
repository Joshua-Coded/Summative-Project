# Path Finding Application

## Overview
The Path Finding Application is a C-based tool that helps users find the shortest path between cities in a transportation network. It implements Dijkstra's algorithm to calculate the most efficient route based on travel time between connected locations.

## Features
- Case-insensitive city name matching
- Interactive menu system
- Displays all available cities in the network
- Shows detailed path information with segment-by-segment travel times
- Handles user input errors gracefully

## Technical Implementation

### Data Structures
- **Adjacency Matrix**: Represents the graph of cities and connections
- **City Names Array**: Stores the names of all cities in the network
- **Distance Array**: Tracks shortest known distance to each node during pathfinding
- **Previous Nodes Array**: Records the optimal path for reconstruction

### Algorithms

#### Dijkstra's Algorithm
The application uses Dijkstra's algorithm to find the shortest path between two cities:

1. Initialize all distances as infinite except the source node (0)
2. Mark all nodes as unvisited
3. For each iteration:
   - Select the unvisited node with the smallest distance
   - Mark it as visited
   - Update distances to all adjacent nodes if a shorter path is found
4. Continue until destination is reached or no paths remain

**Time Complexity**: O(V²) where V is the number of vertices (cities)
- **Best Case**: O(V²) - Must check all vertices
- **Worst Case**: O(V²) - Must check all vertices and edges

**Space Complexity**: O(V²) for the adjacency matrix + O(V) for auxiliary arrays

#### City Name Matching
The application implements case-insensitive and whitespace-tolerant city name matching:

1. Normalize input by converting to lowercase and trimming spaces
2. Compare normalized strings for matching
3. Return the index of the matched city or -1 if not found

**Time Complexity**: O(n) where n is the number of cities
- **Best Case**: O(1) - First city matches
- **Worst Case**: O(n) - Last city matches or no match found

## Usage Instructions

### Installation
1. Compile the application using the provided Makefile:
   ```bash
   make
   ```

2. Run the executable:
   ```bash
   ./path_finding
   ```

### Using the Application

#### Main Menu Options
The application presents three options:
1. **Find shortest path between cities**
2. **View all available cities**
3. **Exit**

#### Finding the Shortest Path
1. Select option 1 from the main menu
2. Enter the starting city (case-insensitive)
3. Enter the destination city (case-insensitive)
4. View the calculated route and total travel time

#### Example Session
```
=============================================
       CITY PATH FINDING APPLICATION        
=============================================

Options:
1. Find shortest path between cities
2. View all available cities
3. Exit
Choose an option (1-3): 1

Enter start city: bumbogo
Enter destination city: airport

Shortest path from Bumbogo to Airport:
------------------------------
Bumbogo -> Kanombe (30 min) -> Airport (5 min)
Total travel time: 35 minutes
```

## Performance Considerations

### Best Case Scenarios
- Small network with direct connections between most cities
- Queries between nearby cities with few hops
- Well-distributed network with multiple path options

### Worst Case Scenarios
- Large network with many cities (approaches MAX_CITIES limit)
- Highly disconnected graph requiring many path calculations
- Queries between distant cities requiring traversal of many edges

### Limitations
- Maximum number of cities is fixed at compile time (default: 100)
- Travel times are integers (minutes)
- No support for directional routes (all connections are bidirectional)
- No support for dynamic network updates during runtime

## Error Handling
- Displays helpful error messages when cities aren't found
- Provides list of available cities when an invalid city is entered
- Handles invalid menu selections gracefully
- Prevents buffer overflows in city name input

## Future Improvements
- Support for weighted directional edges (one-way routes)
- Dynamic addition/removal of cities and routes
- Alternative pathfinding algorithms (A*, Bellman-Ford)
- Visualization of the network and calculated routes
