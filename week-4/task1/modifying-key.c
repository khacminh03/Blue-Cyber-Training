#include<windows.h>
#include<stdio.h>

int main() {
    HKEY hKey;
    LPCSTR subkey = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Hex-Rays SA\\IDA FREEWARE 8.4";
    LPCSTR valueName = "Daru";
    DWORD dwDescription;
    DWORD dwValue = 1234;
    if (RegCreateKeyEx(HKEY_CURRENT_USER, subkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDescription) == ERROR_SUCCESS) {
        printf("Registry key created success\n");
        if (RegSetValueEx(hKey, valueName, 0, REG_DWORD, (const BYTE*)&dwValue, sizeof(dwValue)) == ERROR_SUCCESS) {
            printf("Registry key value set successfully!\n");
        } else {
            printf("Failed to create registry key\n");
        }
        RegCloseKey(hKey);
    } else {
        printf("Failed to create registry key\n");
    }
}