#include<stdio.h>
#include<windows.h>

BOOL APIENTRY DllMain(HMODULE hModdule, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, "This is real dll", "Real Dll", MB_OK);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
__declspec(dllexport) void realFunction() {
    printf("You have not been hacked\n");
}