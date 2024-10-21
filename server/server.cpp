

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define MAX_REQUESTS 10

// stores the shortest paths
unordered_map<int, vector<int>> shortest_paths; 

// the function reads the graph from the given file and saves it as a matrix
vector<vector<int>> readGraphFromFile(const char* filename) {
    vector<vector<int>> graph;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    int u, v;
    while (file >> u >> v) {
        while (max(u, v) >= graph.size())
            graph.emplace_back();
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    file.close();
    return graph;
}

// the function finds the shortest path between two nodes by using BFS algorithm
vector<int> shortestPath(const vector<vector<int>>& graph, int start, int end) {
    if (start < 0 || start >=graph.size() || end < 0 || end>= graph.size()) {
        cerr << "Invalid vertex index" << endl;
        return vector<int>(); // Return empty path
    }

    if (shortest_paths.find(start* graph.size() + end) != shortest_paths.end())
        return shortest_paths[start * graph.size() + end];
    
    vector<int> path;
    vector<bool> visited(graph.size(), false);
    vector<int> prev(graph.size(), -1);
    queue<int> q;

    q.push(start);
    visited[start]= true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == end) {
            int node = current;
            while (node !=-1) {
                path.insert(path.begin(), node);
                node = prev[node];
            }
            shortest_paths[start * graph.size() + end] = path;
            return path;
        }

        for (int neighbor : graph[current]) {
    if (neighbor < 0 || neighbor >= graph.size()) {
        cerr << "Invalid neighbor" << endl;
        return vector<int>(); // Return empty path
    }
    if (!visited[neighbor]) {
        q.push(neighbor);
        visited[neighbor] = true;
        prev[neighbor] = current;
    }
}


    }
    return path; // no path found
}


//the function handles the client requests
void handleClient(int client_socket, const vector<vector<int>>& graph) {
    int vertices[2];
    recv(client_socket, &vertices, sizeof(vertices), 0);
    int v1 = vertices[0];
    int v2 = vertices[1];
    vector<int> path = shortestPath(graph, v1, v2);
    send(client_socket, &path[0], path.size() * sizeof(int), 0);
    close(client_socket);
}



int main(int argc, char* argv[]) {
    
    // Checks the number of arguments in the command line
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <filename> <port>" << endl;
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];
    int port = atoi(argv[2]);
    
    // Reading graph from the file
    vector<vector<int>> graph = readGraphFromFile(filename);

    // Create server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cerr << "Error creating server socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Bind server socket to address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cerr << "Error binding socket" << endl;
        exit(EXIT_FAILURE);
    } //else {
        //cerr <<"connecting to the client..." << endl;
    //}

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        cerr << "Error listening on socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Accept and handle client connections
    while (true) {
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        if (client_socket == -1) {
            cerr << "Error accepting client connection" << endl;
            continue;
        }

        handleClient(client_socket, graph);
    }

    close(server_socket);
    return 0;
}





