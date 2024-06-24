#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "lws2_32")
int prefix(const char *pre, const char *command) {
    return strncmp(pre, command, strlen(pre)) == 0;
}

// function after received command 'puts + filename'
void StartReceivedFile(SOCKET serverSocket, const char *filename) {
    char fileSizeReceived[10];
    memset(fileSizeReceived, 0, sizeof(fileSizeReceived));
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int received = 0;

    if ((received = recv(serverSocket, fileSizeReceived, sizeof(fileSizeReceived), 0)) < 0) {
        printf("Failed to received file size!\n");
    } 

    printf("Received size file from server: %s\n", fileSizeReceived);

    long int fileSize = atol(fileSizeReceived);
    memset(buffer, 0, sizeof(buffer));
    FILE *file = fopen(filename, "wb");
    int total = 0;
    int count = 0;
    while(total < fileSize) {
        received = recv(serverSocket, buffer, sizeof(buffer), 0);
        printf("[Debug] received %d and count %d\n", received, count++);
        // printf("Read data in buffer: \n");
        // for (int i = 0; i < strlen(buffer); i++) {
        //     printf("%02x ", buffer[i]);
        // }
        printf("\n");
        fwrite(buffer, 1, received, file);
        total += received;  
    }
    fclose(file);
    printf("Received file success\n");
    return;
}
// function after received command 'gets + filename'
void StartSendFile(SOCKET serverSocket, const char *filename) {
    char fileSizeToSend[10];
    memset(fileSizeToSend, 0, sizeof(fileSizeToSend));
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int received = 0;
    long int fileSize = 0;

    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("[+] File %s has %ld kb\n", filename, fileSize);
    snprintf(fileSizeToSend, sizeof(fileSizeToSend), "%ld", fileSize);
    printf("[+] changed from long int to string currently value is %s\n", fileSizeToSend);
    printf("[+] Start to send file to server!\n");
    send(serverSocket, fileSizeToSend, sizeof(fileSizeToSend), 0);
    Sleep(1000);

    int read = 0;
    int count = 0;
    int total = 0;

    while(total < fileSize) {
        read = fread(buffer, 1, sizeof(buffer), file);
        send(serverSocket, buffer, read, 0);
        total += read;
    }
    fclose(file);
    printf("[+] Send file to server success!\n");
    return;
}
int main () {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[4096];
    int receiveSize;
    memset(buffer, 0, sizeof(buffer));
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed error code: %d\n", WSAGetLastError());
    }
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("could not create socket: %d\n", WSAGetLastError());
    }
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connect failed: %d\n", WSAGetLastError());
    }

    while(1) {
        if ((receiveSize = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR) {
            printf("Receive failed: %d\n", WSAGetLastError());
            break;
        }
        buffer[receiveSize] = '\0';
        printf("Received command: %s\n", buffer);
        if (prefix("puts ", buffer)) {
            printf("Starting function to receive file\n");
            char filename[4096];
            sscanf(buffer + 5, "%s", filename);
            StartReceivedFile(sock, filename);
        } else if (prefix("gets ", buffer)) {
            printf("Starting function to send file to server\n");
            char filename[4096];
            sscanf(buffer + 5, "%s", filename);
            StartSendFile(sock, filename);
        } else {
            printf("Bull shit command!");
            break;
        }
    }
}