#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    int recv_size;

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        send(client_socket, buffer, strlen(buffer), 0);

        recv_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (recv_size > 0) {
            buffer[recv_size] = '\0';
            printf("%s\n", buffer);
        } else if (recv_size == 0) {
            printf("Connection closed\n");
            break;
        } else {
            printf("Recv failed with error code: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(client_socket);
}

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_socket;
    struct sockaddr_in server, client;
    int c = sizeof(struct sockaddr_in);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        return 1;
    }

    listen(server_fd, 3);

    printf("Server listening on port %d\n", PORT);

    while ((client_socket = accept(server_fd, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {
        handle_client(client_socket);
    }

    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed with error code: %d\n", WSAGetLastError());
        return 1;
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}
