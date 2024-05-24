#include <stdio.h>
#include <windows.h>
int main(int argc, char *argv[]) {
    DWORD       TID               = 0;
	DWORD       PID               = 0;
	LPVOID      rBuffer           = NULL;
	HANDLE      hProcess          = NULL;
	HANDLE      hThread           = NULL;
	HMODULE     hKernel32         = NULL;
	wchar_t     dllPath[MAX_PATH] = L".\\dllInjector.dll";
	size_t      pathSize          = sizeof(dllPath);
	size_t      bytesWritten      = 0;


    if (argc < 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }

    PID = atoi(argv[1]);
    printf("[DEBUG] pid = %d", PID);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (hProcess == NULL) {
        printf("[DEBUG] unable to get a handle to the process (%ld), error: 0x%lx\n", PID, GetLastError());
    }
    printf("[DEBUG] got a handle to the process\n");

    printf("Getting handle to kernel32.dll\n");
    hKernel32 = GetModuleHandleW(L"kernel32");
    if (hKernel32 == NULL) {
        printf("Failed to load kernel32, error: 0x%lx\n", GetLastError());
        return EXIT_FAILURE;
    }
    printf("[DEBUG] got a kernel32\n");

    printf("[DEBUG] Getting address of LoadLibraryW()\n");
    LPTHREAD_START_ROUTINE kawLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
    printf("[DEBUG] got address of LoadLibraryW()\n");

    printf("allocating memory in target process\n");
    rBuffer = VirtualAllocEx(hProcess, NULL, pathSize, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
    if (rBuffer == NULL) {
        printf("Can not allocate a buffer to the target process memory, error: 0x%lx\n", GetLastError());
        goto CLEANUP;
    }
    printf("Allocated buffer\n");

    printf("Writing to buffer\n");
    WriteProcessMemory(hProcess, rBuffer, dllPath, pathSize, &bytesWritten);
    printf("Done with WriteProcessMemory\n");

    printf("Creating a thread\n");
    hThread = CreateRemoteThread(hProcess, NULL, 0, kawLoadLibrary, rBuffer, 0, &TID);

	if (hThread == NULL) {
		printf("unable to create thread, error: 0x%lx\n", GetLastError());
		goto CLEANUP;
	}

    printf("created a new thread\n");
    printf("Waiting thread finish\n");
    WaitForSingleObject(hThread, INFINITE);
    printf("Thread done\n");
    
    CLEANUP:
        if (hThread) {
            printf("closing handle to thread\n");
            CloseHandle(hThread);
        }

        if (hProcess) {
            printf("closing handle to process\n");
            CloseHandle(hProcess);
        }

        printf("finished with house keeping, see you next time!\n");
        return EXIT_SUCCESS;
}
