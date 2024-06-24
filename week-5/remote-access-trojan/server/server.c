#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

int prefix(const char *pre, const char *str) {
    return strncmp(pre, str, strlen(pre)) == 0;
}

void send_file(SOCKET sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    char storedFileSize[1];
    memset(storedFileSize, 0, sizeof(storedFileSize));
    long long int file_size;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    
    printf("File size is %lld\n", file_size);
    fseek(file, 0, SEEK_SET);
    snprintf(storedFileSize, 1, "%lld", file_size);
    printf("Change buffer after using snprintf: %s\n", storedFileSize);
    send(sock, storedFileSize, strlen(buffer), 0);
    printf("Send file size done!\n");
    memset(buffer, 0, BUFFER_SIZE);

    int count = 0;
    int total = 0;
    int readFile;
    while ((readFile = fread(buffer, 1, file_size, file)) > 0) {
        // printf("Still sending %d and send dataset %d\n", count++, readFile);
        if (send(sock, buffer, readFile, 0) < 0) {
            printf("Failed to send file: %d\n", WSAGetLastError());
            break;
        }
    }
    fclose(file);
    snprintf(buffer, BUFFER_SIZE, "%ld", -100000);
    send(sock, buffer, strlen(buffer), 0);
    printf("Done and done\n");
}
void get_file(SOCKET sock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int received;
    int count = 0;
    if ((received = recv(sock, buffer, sizeof(buffer), 0)) <= 0) {
        printf("Failed to receive file size\n");
        return;
    }
    buffer[received] = '\0';
    long long int file_size = atoll(buffer);
    printf("Received file size: %lld\n", file_size);
    memset(buffer, 0, sizeof(buffer));
    if (file == NULL) {
        printf("Failed to open file\n");
    }
    int total = 0;
    while(total < file_size && received != -100000) {
        received = recv(sock, buffer, BUFFER_SIZE, 0);
        printf("Received data from file %d and still received: %d\n", received, count);
        if (received <= 0) {
            printf("Recv failed: %d\n", WSAGetLastError());
            break;
        }
        fwrite(buffer, 1, received, file);
        count++;
        total += received;
        // printf("Now getting %d\n", total);
    }
    fclose(file);
    printf("Get the fucking file success\n");
    return;
}

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int recv_size;

    while (1) {
        printf("> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (prefix("puts ", buffer)) {
            send(client_socket, buffer, strlen(buffer), 0);
            printf("Start send file\n");
            char filename[BUFFER_SIZE];
            sscanf(buffer + 5, "%s", filename);
            send_file(client_socket, filename);
        } else if (prefix("gets ", buffer)) {
            send(client_socket, buffer, strlen(buffer), 0);
            printf("Start received file from client!\n");
            char filename[BUFFER_SIZE];
            sscanf(buffer + 5, "%s", filename);
            get_file(client_socket, filename);
        } else {
            if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
                printf("Send failed with error code: %d\n", WSAGetLastError());
                break;
            }
        }

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
