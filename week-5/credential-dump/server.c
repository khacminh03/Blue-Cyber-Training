#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_len;
    char *message;
    char client_ip[1024];
    char capture;
    char char_to_str[2]; // Array to convert char to string
    char buffer[1024];
    int recv_size;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Listen
    listen(server_socket, 3);

    // Accept an incoming connection
    printf("Waiting for incoming connections...\n");
    client_addr_len = sizeof(struct sockaddr_in);

    // Accept connection from an incoming client
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        return 1;
    }

    // Convert the client's IP address to a string
    strcpy(client_ip, inet_ntoa(client_addr.sin_addr));
    printf("Connection accepted from: %s\n", client_ip);

    // request from client
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv_size = recv(client_socket, buffer, sizeof(buffer), 0);
        if (recv_size == SOCKET_ERROR) {
            printf("Recv failed: %d\n", GetLastError());
            break;
        } else if (recv_size == 0) {
            printf("Client disconnected\n");
            break;
        }
        buffer[recv_size] = '\0';
        printf("Received: %s\n", buffer);
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
