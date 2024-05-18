#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <ProcessID>\n", argv[0]);
        return 1;   
    }

    DWORD processID = atoi(argv[1]);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL)
    {
        printf("Failed to open process. Error code: %lu\n", GetLastError());
        return 1;
    }

    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    MEMORY_BASIC_INFORMATION memoryInfo;
    LPVOID memoryAddress = systemInfo.lpMinimumApplicationAddress;

    printf("Memory regions with PAGE_EXECUTE_READWRITE protection in process with ID %lu:\n", processID);
    while (memoryAddress < systemInfo.lpMaximumApplicationAddress)
    {
        if (VirtualQueryEx(hProcess, memoryAddress, &memoryInfo, sizeof(memoryInfo)) == sizeof(memoryInfo))
        {
            if (memoryInfo.Protect == PAGE_EXECUTE_READWRITE)
            {
                printf("Base Address: %p\n", memoryInfo.BaseAddress);
                printf("Region Size: %zu bytes\n", memoryInfo.RegionSize);
                printf("State: %lu\n", memoryInfo.State);
                printf("Type: %lu\n", memoryInfo.Type);
                printf("\n");
            }
            memoryAddress = (LPVOID)((char *)memoryAddress + memoryInfo.RegionSize);
        }
        else
        {
            printf("Failed to query memory information. Error code: %lu\n", GetLastError());
            break;
        }
    }

    CloseHandle(hProcess);
    return 0;
}
