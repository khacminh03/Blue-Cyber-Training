#include<stdio.h>
#include<windows.h>
BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReversed) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, "Hi, I am a DLL", "DLL message", MB_OK | MB_ICONINFORMATION);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}