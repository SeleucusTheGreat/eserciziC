#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080

void main () {
    int serversoc;
    struct sockaddr_in server_address;

    if((serversoc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error opening the socket");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(serversoc, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("Error connecting to the server");
        return;
    }

    printf("Connection established\n");

    char* str = "hello server\n";
    send(serversoc, str, strlen(str), 0);

    printf("Message sent\n");

    char buffer[1024] = {0};
    read(serversoc, buffer, sizeof(buffer));

    printf("Received message: %s", buffer);

    close(serversoc);

    printf("Connection closed\n");

    return;
}
