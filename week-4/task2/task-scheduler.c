#include <windows.h>
#include <tchar.h>
#include <stdio.h>
void create_scheduled_task() {
    TCHAR command[] = TEXT("E:\\blue-cyber-re\\week-4\\task2\\scheduler.bat");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    } else {
        printf("Run success\n");
    }
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    create_scheduled_task();
    return 0;
}
