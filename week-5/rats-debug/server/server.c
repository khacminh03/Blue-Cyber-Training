#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>
#pragma comment (lib, "lws2_32")

int prefix(const char *pre, const char *command) {
    return strncmp(pre, command, strlen(pre)) == 0;
}

// function for command gets + filename
void StartReceiveFile(SOCKET clientSocket, const char *filename) {
    char fileSizeReceived[10];
    memset(fileSizeReceived, 0, sizeof(fileSizeReceived));
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int received = 0;

    if ((received = recv(clientSocket, fileSizeReceived, sizeof(fileSizeReceived), 0)) < 0) {
        printf("Failed to received file size!\n");
    }

    printf("Received file size from client: %s\n", fileSizeReceived);
    printf("Received file name: %s\n", filename);
    long int fileSize = atol(fileSizeReceived);
    FILE *file = fopen(filename, "wb");
    int total = 0;
    int count = 0;
    while(total < fileSize) {
        received = recv(clientSocket, buffer, sizeof(buffer), 0);
        printf("[Debug] received %d and count %d\n", received, count++);
        // printf("Read data from buffer\n");
        // for (int i = 0; i < strlen(buffer); i++) {
        //     printf("%02x ", buffer[i]);
        // }
        printf("\n");
        fwrite(buffer, 1, received, file);
        total += received;
    }
    fclose(file);
    printf("Received file from client success\n");
    return;
}
// function for command puts + filename
void StartSendFile(SOCKET clientSocket, const char *filename) {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    char sendSizeOfFile[10];
    memset(sendSizeOfFile, 0, sizeof(sendSizeOfFile));
    long int fileSize = 0;

    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    printf("[+] Size of file %s has %ld kb\n", filename, fileSize);
    fseek(file, 0, SEEK_SET);
    snprintf(sendSizeOfFile, sizeof(sendSizeOfFile), "%ld", fileSize);
    printf("changed from long int to string currently value is %s\n", sendSizeOfFile);
    printf("Start to send file size to client!\n");
    send(clientSocket, sendSizeOfFile, sizeof(sendSizeOfFile), 0);
    Sleep(1000);

    int read = 0;
    int count = 0;
    int total = 0;
    while(total < fileSize) {
        int read = fread(buffer, 1, sizeof(buffer), file);
        send(clientSocket, buffer, read, 0);
        total += read;
    }
    fclose(file);
    printf("Send file to client success!\n");
    return;
}
int main () {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int c = sizeof(struct sockaddr_in);
    char buffer[4096];

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error code: %d\n", WSAGetLastError());
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        return 1;
    }

    listen(serverSocket, 3);

    printf("Server listening on port 8080\n");
    while ((clientSocket = accept(serverSocket, (struct sockaddr *)&client, &c)) != INVALID_SOCKET) {
        while(1) {
            memset(buffer, 0, sizeof(buffer));
            printf("# ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            printf("Server send: %s\n", buffer);
            send(clientSocket, buffer, strlen(buffer), 0);
            if (prefix("puts ", buffer)) {
                printf("Start send file from server to client\n");
                char filename[4096];
                memset(filename, 0, sizeof(filename));
                sscanf(buffer + 5, "%s", filename);
                StartSendFile(clientSocket, filename);
            } else if (prefix("gets ", buffer)) {
                printf("Start received file from client to server\n");
                char filename[4096];
                memset(filename, 0, sizeof(filename));
                sscanf(buffer + 5, "%s", filename);
                StartReceiveFile(clientSocket, filename);
            }
        }
    }

}