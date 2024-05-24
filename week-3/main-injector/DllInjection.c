#include <windows.h>
#include <stdio.h>

BOOL __stdcall DllMain (HINSTANCE hModule, DWORD dwReason, LPVOID lpvReversed) {
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, "You have been hacked now pay me 1 million dollars", "Hacker warn", MB_ICONEXCLAMATION);
        break;
    }
    return TRUE;
}