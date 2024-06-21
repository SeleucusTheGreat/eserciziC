#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

#define SOCKET_PATH "./myapp.sock" // Custom path for the Unix domain socket
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_un address;
    char buffer[BUFFER_SIZE] = {0};
    pid_t pid;

    // Create socket file descriptor
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        error("socket failed");
    }

    // Configure address
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);
    unlink(SOCKET_PATH); 
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        error("bind failed");
    }

    // Fork the process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        error("fork failed");
    } else if (pid == 0) {
        // Child process - Client
        close(server_fd);  // Close unused server file descriptor

        int sock = 0;
        struct sockaddr_un serv_addr;
        char *hello = "Hello from client";

        if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            error("Socket creation error");
        }

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sun_family = AF_UNIX;
        strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
            error("Connection failed");
        }

        send(sock, hello, strlen(hello) + 1, 0);
        printf("Client sent message: %s\n", hello);
        int valread = read(sock, buffer, BUFFER_SIZE);
        printf("Client received message: %s\n", buffer);
        close(sock);
    } else {
        // Parent process - Server
        if (listen(server_fd, 3) == -1) {
            error("listen failed");
        }

        if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
            error("accept failed");
        }

        int valread = read(client_fd, buffer, BUFFER_SIZE);
        printf("Server received message: %s\n", buffer);
        char *response = "Hello from server";
        send(client_fd, response, strlen(response) + 1, 0);
        printf("Server sent message: %s\n", response);
        close(client_fd);
        close(server_fd);

        // Wait for child process to complete
        wait(NULL);

        // Clean up the socket file
        unlink(SOCKET_PATH);
    }

    return 0;
}
