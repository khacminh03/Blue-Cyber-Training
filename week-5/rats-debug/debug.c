#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    long int res = 0;
    FILE *file = fopen("awa.pdf", "rb");
    if (file == NULL) {
        printf("file not found\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    res = ftell(file);
    fseek(file, 0, SEEK_SET);
    int total = 0;
    int count = 0;
    while(total < res && count <= 5) {
        int read = fread(buffer, 1, sizeof(buffer), file);  // read returns the number of bytes read
        if (read <= 0) {
            break;  // break if read fails
        }
        total += read;
        printf("Total: %d\n", total);
        printf("Read: %d\n", read);

        // Optionally, print the buffer content as hex for better debugging
        printf("Buffer: ");
        for(int i = 0; i < read; i++) {
            printf("%02x ", (unsigned char)buffer[i]);
        }
        printf("\n");

        memset(buffer, 0, sizeof(buffer));
        count++;
    }
    fclose(file);
    printf("Size of file: %ld\n", res);
    return 0;
}
