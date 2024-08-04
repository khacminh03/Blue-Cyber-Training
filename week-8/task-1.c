#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s username\n", argv[0]);
        return 0;
    }
    char *username = argv[1];
    char cmd[100];
    sprintf(cmd, "sudo passwd %s", username);
    system(cmd);
}