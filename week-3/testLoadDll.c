#include<stdio.h>
#include<windows.h>
int main() {
    HMODULE hDll = LoadLibrary("loadDll.dll");
    if (hDll == NULL) {
        printf("Failed to load the dll file.\n");
    } else {
        printf("Success\n");
    }
    FreeLibrary(hDll);
}