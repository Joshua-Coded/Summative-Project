# Forensic Friend Detection System - Technical Documentation

## Project Overview

The Forensic Friend Detection System is a specialized tool designed for criminal investigations that builds and analyzes social connection graphs based on phone call records. The system represents each phone number as a node in a graph and each call connection as an edge between nodes, allowing investigators to visualize networks of communications and identify direct contacts of persons of interest.

## Table of Contents
1. [System Architecture](#system-architecture)
2. [Algorithm Analysis](#algorithm-analysis)
3. [Data Structures](#data-structures)
4. [Implementation Details](#implementation-details)
5. [Time & Space Complexity Analysis](#time--space-complexity-analysis)
6. [User Interface](#user-interface)
7. [Compilation & Execution](#compilation--execution)
8. [Future Enhancements](#future-enhancements)

## System Architecture

The system is built using a modular C programming architecture with the following components:

### File Structure
- `friend_detection.h` - Header file containing data structures and function prototypes
- `friend_detection.c` - Implementation of core graph and contact management functions
- `main.c` - User interface and application control flow
- `Makefile` - Compilation configuration

### Core Components
1. **Graph Management** - Handles the creation and manipulation of the adjacency matrix
2. **Contact Database** - Manages phone numbers and associated identity information
3. **Query Interface** - Processes user requests for connection information
4. **Visualization System** - Displays the network in human-readable formats

## Algorithm Analysis

### Graph Construction Algorithm
The system uses an adjacency matrix representation for the graph, where each cell M[i][j] indicates whether there is a connection between nodes i and j.

```
Function: addEdge(graph, contacts, caller, callee, nodeCount)
    1. Find caller in contacts or add if not present
    2. Find callee in contacts or add if not present
    3. Set graph[callerIndex][calleeIndex] = 1
    4. Set graph[calleeIndex][callerIndex] = 1 (undirected graph)
```

**Time Complexity**: O(n) for finding/adding each node, where n is the current number of nodes.

### Direct Contact Search Algorithm
To find all direct contacts of a target phone number:

```
Function: findDirectContacts(graph, contacts, nodeCount, targetNumber)
    1. Find the index of targetNumber in contacts
    2. For each node i from 0 to nodeCount-1:
        a. If graph[targetIndex][i] == 1, output contacts[i]
```

**Time Complexity**: O(n) to find the target + O(n) to scan all possible connections = O(n).

### Contact Search Algorithm
To search for contacts by phone number or name fragment:

```
Function: searchContact(contacts, nodeCount)
    1. Get search term from user
    2. For each contact i from 0 to nodeCount-1:
        a. If search term is substring of contact's phone or name:
            i. Output contact details
```

**Time Complexity**: O(n·m) where n is the number of contacts and m is the average string length.

## Data Structures

### Contact Structure
```c
typedef struct {
    char phone[15];
    char name[MAX_NAME_LENGTH];
} Contact;
```

### Graph Representation
The system uses an adjacency matrix for the graph:
```c
int graph[MAX_NODES][MAX_NODES];
```
- A value of 1 at `graph[i][j]` indicates a connection between nodes i and j
- A value of 0 indicates no connection

## Implementation Details

### Node Management
The system maintains a mapping between phone numbers and their indices in the adjacency matrix using the Contact array. When adding new connections, the system:
1. Checks if both phone numbers already exist in the contacts array
2. Adds any new phone numbers to the contacts array
3. Updates the adjacency matrix to reflect the connection

### User Interface Implementation
The user interface is implemented using a menu-driven approach with the following components:
1. **Main Menu** - Presents options for various operations
2. **Query Interface** - Accepts user input for specific searches
3. **Results Display** - Formats and presents query results
4. **Visual Enhancements** - Uses ANSI color codes for improved readability

## Time & Space Complexity Analysis

### Time Complexity
- **Graph Initialization**: O(n²) where n is MAX_NODES
- **Adding an Edge**: O(n) for finding/adding nodes
- **Finding Direct Contacts**: O(n)
- **Displaying Adjacency Matrix**: O(n²)
- **Searching Contacts**: O(n·m) where m is average string length

### Space Complexity
- **Adjacency Matrix**: O(n²) where n is MAX_NODES
- **Contact Array**: O(n·k) where k is maximum string length
- **Total Space Complexity**: O(n²) dominated by the adjacency matrix

## User Interface

The system provides a professional text-based interface with the following features:
1. **Color-Coded Output** - Uses ANSI color codes for better visual differentiation
2. **Clear Menu System** - Intuitive navigation with numbered options
3. **Formatted Data Display** - Tabular representation of contacts and matrix
4. **Search Functionality** - Flexible search for finding contacts

### Main Menu Options
1. Find Direct Contacts for a Phone Number
2. Display All Contacts
3. Search Contact Database
4. Display Network Adjacency Matrix
5. Exit

## Compilation & Execution

### Prerequisites
- GCC compiler
- Make utility

### Compilation
```bash
make
```

The Makefile includes the following configurations:
```makefile
CC = gcc
CFLAGS = -Wall -g -Iinclude
LDLIBS =

SRC_FILES = src/friend_detection.c main.c
OBJS = $(SRC_FILES:.c=.o)
EXEC = friend_detection
```

### Execution
```bash
./friend_detection
```
