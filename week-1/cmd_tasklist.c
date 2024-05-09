#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

void printProcessList() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Lấy snapshot của tất cả các tiến trình trong hệ thống
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("CreateToolhelp32Snapshot failed.\n");
        return;
    }

    // Thiết lập kích thước của cấu trúc PROCESSENTRY32
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Lấy thông tin về tiến trình đầu tiên
    if (!Process32First(hProcessSnap, &pe32)) {
        printf("Process32First failed.\n");
        CloseHandle(hProcessSnap);
        return;
    }

    // In tiêu đề
    printf("%-25s %s\n", "Image Name", "PID");
    printf("-----------------------------------\n");

    // In thông tin về từng tiến trình
    do {
        printf("%-25s %lu\n", pe32.szExeFile, pe32.th32ProcessID);
    } while (Process32Next(hProcessSnap, &pe32));

    // Đóng handle của snapshot
    CloseHandle(hProcessSnap);
}

int main() {
    printProcessList();
    return 0;
}
