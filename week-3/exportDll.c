#include<stdio.h>
#include<windows.h>
__declspec(dllexport) void printArgument(const char *input) {
    MessageBox(NULL, input, "DLL message", MB_OK | MB_ICONINFORMATION);
}
__declspec(dllexport) void writeFile(const char *input) {
    FILE *fptr;
    fptr = fopen(input, "w");
    if (fptr == NULL) {
        MessageBox(NULL, "Failed to open file", "DLL Error", MB_OK | MB_ICONERROR);
    }
    fprintf(fptr, "You have been hacked!");
    fclose(fptr);
}