#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];
    int port_number;

    // 1. Configurable Port Check
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    port_number = atoi(argv[1]);

    // 2. Create Socket
    // AF_INET = IPv4, SOCK_STREAM = TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("Socket creation failed");
    }

    // Options to reuse the address/port (prevents "Address already in use" errors)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        error("setsockopt failed");
    }

    // 3. Bind Socket to Port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    address.sin_port = htons(port_number);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error("Bind failed");
    }

    // 4. Listen (Queue up to 5 connections)
    if (listen(server_fd, 5) < 0) {
        error("Listen failed");
    }

    printf("Server listening on port %d...\n", port_number);

    // 5. Main Loop to Accept Clients
    while (1) {
        client_len = sizeof(client_addr);
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected.\n");

        // 6. Handle Communication
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            
            // Receive data
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            
            // Handle Disconnect (0) or Error (-1)
            if (valread <= 0) {
                printf("Client disconnected unexpectedly.\n");
                break;
            }

            // Remove newline char if present for comparison
            buffer[strcspn(buffer, "\n")] = 0;

            printf("Client sent: %s\n", buffer);

            // Logic: Echo back the message
            send(new_socket, buffer, strlen(buffer), 0);

            // Logic: Check for EXIT command
            if (strncmp(buffer, "EXIT", 4) == 0) {
                printf("Exit command received. Closing connection.\n");
                break;
            }
        }
        
        close(new_socket);
        printf("Waiting for new connection...\n");
    }

    return 0;
}