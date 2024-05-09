#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <tlhelp32.h>


bool terminate_by_pid(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        printf("Failed to open process with PID %lu\n", pid);
        return false;
    }
    
    if (!TerminateProcess(hProcess, 0)) {
        printf("Failed to terminate process with PID %lu\n", pid);
        CloseHandle(hProcess);
        return false;
    }
    
    printf("Terminated process with PID %lu\n", pid);
    CloseHandle(hProcess);
    return true;
}


bool terminate_by_image_name(const char* image_name) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        printf("Failed to create process snapshot\n");
        return false;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe)) {
        printf("Failed to get first process in snapshot\n");
        CloseHandle(hSnap);
        return false;
    }

    bool terminated = false;
    do {
        if (_stricmp(pe.szExeFile, image_name) == 0) {
            terminated |= terminate_by_pid(pe.th32ProcessID);
        }
    } while (Process32Next(hSnap, &pe));

    CloseHandle(hSnap);
    return terminated;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <method> <value>\n", argv[0]);
        printf("    <method>: 'image' or 'pid'\n");
        printf("    <value>: image name or PID\n");
        return 1;
    }

    const char* method = argv[1];
    const char* value = argv[2];

    if (_stricmp(method, "image") == 0) {
        if (!terminate_by_image_name(value)) {
            printf("No process found with image name '%s'\n", value);
            return 1;
        }
    } else if (_stricmp(method, "pid") == 0) {
        DWORD pid = atoi(value);
        if (pid == 0) {
            printf("Invalid PID\n");
            return 1;
        }
        if (!terminate_by_pid(pid)) {
            printf("No process found with PID %lu\n", pid);
            return 1;
        }
    } else {
        printf("Invalid method. Use 'image' or 'pid'.\n");
        return 1;
    }

    return 0;
}
