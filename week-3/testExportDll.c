#include<stdio.h>
#include<windows.h>
#include "exportDll.h"
typedef void (*PrintArgFunc)(char*);
typedef void (*WriteFileFunc)(char*);
int main() {
    HMODULE hDll = LoadLibrary("exportDll.dll");
    if (hDll == NULL) {
        printf("Failed to load dll file");
    }
    PrintArgFunc printArgument = (PrintArgFunc)GetProcAddress(hDll, "printArgument");
    if (printArgument == NULL) {
        printf("Failed to find function: printArgument\n");
        FreeLibrary(hDll);
    }
    printArgument("i will encrypt your data unless you pay me 1 million dollars");
    WriteFileFunc writeFile = (WriteFileFunc)GetProcAddress(hDll, "writeFile");
    if (writeFile == NULL) {
        printf("Failed to find function : writefile\n");
        FreeLibrary(hDll);
    }
    writeFile("important.txt");
    FreeLibrary(hDll);
}