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
    int clientsoc;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int addr_len = sizeof(client_address);

    if((serversoc = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error opening the socket");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(serversoc, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("Error with the bind");
        close(serversoc);
        return;
    }

    if (listen(serversoc, 2) < 0) {
        perror("Error with the listen");
        close(serversoc);
        return;
    }

    printf("Server is listening on 127.0.0.1:%d\n", PORT);

    if ((clientsoc = accept(serversoc, (struct sockaddr*) &client_address, (socklen_t*) &addr_len)) < 0) {
        perror("Error with the accept");
        close(serversoc);
        return;
    }

    printf("Connection accepted\n");

    char buffer[1024] = {0};
    read(clientsoc, buffer, sizeof(buffer));

    printf("Received message: %s", buffer);

    char* str = "hello client\n";
    send(clientsoc, str, strlen(str), 0);

    close(clientsoc);
    close(serversoc);

    printf("Connection closed\n");

    return;
}






