
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char* argv[]) {
      // Checks the number of arguments in the command line
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <ip> <port> <v1> <v2>" << endl;
        exit(EXIT_FAILURE);
    }

    
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    int vertices[2] = {atoi(argv[3]), atoi(argv[4])};

    // Create a client socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        cerr << "Error creating client socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Connecting to the server
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cerr << "Error connecting to server" << endl;
        exit(EXIT_FAILURE);
    }

    // Sends request to the server
    send(client_socket, &vertices, sizeof(vertices), 0);

    // Receive and print the shortest path from the server
    vector<int> path;
    int buffer;
    while (recv(client_socket, &buffer, sizeof(buffer), 0) > 0) {
        path.push_back(buffer);
    }

    cout << "Shortest path: ";
    for (int node : path) {
        cout << node << " ";
    }
    cout << endl;

    close(client_socket);
    return 0;
}
