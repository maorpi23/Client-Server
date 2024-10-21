# Client-Server Graph Pathfinding Project
This project implements a client-server application that computes the shortest path between two vertices in a graph using the Breadth-First Search (BFS) algorithm. The server reads the graph from a file and processes client requests, while the client connects to the server, sends two vertex indices, and receives the shortest path.

## Files
- client.cpp: The client-side C++ program that sends a request to the server to compute the shortest path between two vertices.
- server.cpp: The server-side C++ program that reads the graph from db.csv, handles client requests, and calculates the shortest path using BFS.
- db.csv: A CSV file representing a graph as edges (pairs of vertices) used by the server to build the graph.

## Key Features
- Client-Server Architecture: The client connects to the server using a TCP socket and sends a start and goal node. The server processes the request and returns the shortest path between the nodes.
- Caching: The server maintains a cache for the 10 most recent path requests, reducing the time needed for repeated requests.
- Multithreading: The server can handle multiple clients simultaneously, with each client served by a dedicated thread.

## Usage
### Compile the server
Navigate to the `server/` directory and compile the server-side code:
```
cd server/
g++ -o server server.cpp
```
