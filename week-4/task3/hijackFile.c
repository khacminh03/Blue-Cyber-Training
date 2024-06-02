#include <windows.h>
#include <stdio.h>

typedef void (*DUMMY_FUNCTION)();

int main() {
    HINSTANCE hinstLib;
    DUMMY_FUNCTION procAdd;
    BOOL fFreeResult;

    hinstLib = LoadLibrary(TEXT("realDll.dll"));

    if (hinstLib != NULL) {

        procAdd = (DUMMY_FUNCTION) GetProcAddress(hinstLib, "realFunction");

        if (NULL != procAdd) {
            procAdd(); 
        } else {
            printf("Could not locate the function.\n");
        }

        fFreeResult = FreeLibrary(hinstLib);
    } else {
        printf("DLL failed to load.\n");
    }

    return 0;
}
