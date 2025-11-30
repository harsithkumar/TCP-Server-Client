#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    
    // 1. Check Arguments of (IP and Port) which is valid or not .
    if (argc < 3) {
        fprintf(stderr,"usage %s invaild port address\n", argv[0]);
        exit(0);
    }

    int port_number = atoi(argv[2]);

    // 2. Create Socket to establish the endpoint between the programs
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("Socket creation error");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_number);//to convert port to network byte order 

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        error("Invalid address/ Address not supported");
    }

    // 3. Connect to Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection Failed");
    }

    printf("Connected to server at %s:%d\n", argv[1], port_number);

    // 4. Communication Loop
    while (1) {
        printf("> ");
        memset(message, 0, BUFFER_SIZE); //clearing buffers
        memset(buffer, 0, BUFFER_SIZE);

        // Get Input
        fgets(message, BUFFER_SIZE, stdin);

        // Remove trailing newline from fgets
        message[strcspn(message, "\n")] = 0;

        // Send to server
        send(sock, message, strlen(message), 0);

        // Check for EXIT to terminate client locally
        if (strncmp(message, "EXIT", 4) == 0) {
            printf("[client exits]\n");
            break;
        }

        // Wait for response
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread > 0) {
            printf("%s\n", buffer);
        } else {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(sock); //Releases the file descriptor.
                //Proper cleanup
    return 0;
}