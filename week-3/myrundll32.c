#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <dll_path> <function_name> [arguments...]\n", argv[0]);
        return 1;
    }

    const char *dllPath = argv[1];
    const char *functionName = argv[2];

    HMODULE hDll = LoadLibraryA(dllPath);
    if (hDll == NULL) {
        fprintf(stderr, "Failed to load DLL: %s\n", dllPath);
        return 1;
    }

    FARPROC fnProc = GetProcAddress(hDll, functionName);
    if (fnProc == NULL) {
        fprintf(stderr, "Failed to find function: %s\n", functionName);
        FreeLibrary(hDll);
        return 1;
    }

    int result = 0;
    if (argc > 3) {
        typedef int(*DLLFunc)(int, char*[]);
        DLLFunc fn = (DLLFunc)(fnProc);
        result = fn(argc - 3, argv + 3);
    } else {
        typedef int(*DLLFunc)();
        DLLFunc fn = (DLLFunc)(fnProc);
        result = fn();
    }

    FreeLibrary(hDll);
    return result;
}
