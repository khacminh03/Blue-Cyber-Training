#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

DWORD GetTargetProcessId(const wchar_t* procname)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            wchar_t wExeFile[MAX_PATH];
            mbstowcs(wExeFile, entry.szExeFile, MAX_PATH);
            
            if (_wcsicmp(wExeFile, procname) == 0)
            {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }
    }

    CloseHandle(snapshot);
    return 0;
}

void InjectDLL(DWORD pid, const char* dllName)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess)
    {
        LPVOID pLibRemote = VirtualAllocEx(hProcess, NULL, strlen(dllName) + 1, MEM_COMMIT, PAGE_READWRITE);
        if (pLibRemote)
        {
            WriteProcessMemory(hProcess, pLibRemote, (void*)dllName, strlen(dllName) + 1, NULL);
            HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), pLibRemote, 0, NULL);
            if (hThread)
            {
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
            }
        }
        CloseHandle(hProcess);
    }
}

int main()
{
    const wchar_t* targetProcess = L"CalculatorApp.exe";
    const char* dllName = "E:\\blue-cyber-re\\week-3\\calculatorMessageBoxInject.dll";

    DWORD pid = GetTargetProcessId(targetProcess);
    if (pid != 0)
    {
        InjectDLL(pid, dllName);
    }
    else
    {
        printf("Could not find target process.\n");
    }

    return 0;
}
