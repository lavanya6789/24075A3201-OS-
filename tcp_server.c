#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Create 
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 1) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP Chat Server Started... Waiting for connections...\n");

    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
    if (client_socket == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("Client: %s", buffer);

        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(client_socket, buffer, strlen(buffer));
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
