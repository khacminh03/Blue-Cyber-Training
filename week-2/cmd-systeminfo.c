#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

void getOSInfo() {
    OSVERSIONINFOEX osvi;
    SYSTEM_INFO si;
    BOOL bOsVersionInfoEx;

    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);

    GetSystemInfo(&si);

    if (bOsVersionInfoEx) {
        printf("OS Name: Windows %ld.%ld\n", osvi.dwMajorVersion, osvi.dwMinorVersion);
        printf("OS Version: %ld.%ld (Build %ld)\n", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
        printf("OS Manufacturer: Microsoft Corporation\n");
        printf("OS Configuration: Standalone Workstation\n");
        printf("OS Build Type: %s\n", osvi.szCSDVersion);
    } else {
        printf("Unable to get OS version information.\n");
    }
}

void getSystemInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    printf("System Type: ");
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            printf("x64-based PC\n");
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            printf("x86-based PC\n");
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            printf("ARM-based PC\n");
            break;
        default:
            printf("Unknown architecture\n");
            break;
    }
}

void getMemoryInfo() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);

    printf("Total Physical Memory: %llu MB\n", statex.ullTotalPhys / 1024 / 1024);
    printf("Available Physical Memory: %llu MB\n", statex.ullAvailPhys / 1024 / 1024);
    printf("Total Virtual Memory: %llu MB\n", statex.ullTotalVirtual / 1024 / 1024);
    printf("Available Virtual Memory: %llu MB\n", statex.ullAvailVirtual / 1024 / 1024);
}

void getProcessorInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    printf("Processor(s):\n");
    printf("    Architecture: ");
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            printf("x64\n");
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            printf("x86\n");
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            printf("ARM\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
    printf("    Number of Processors: %u\n", sysInfo.dwNumberOfProcessors);
    printf("    Processor Type: %u\n", sysInfo.dwProcessorType);
}

void getBootTime() {
    SYSTEMTIME st;
    FILETIME ft;
    ULONGLONG currentTime = 0, bootTime = 0;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    currentTime = ((ULONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;

    bootTime = currentTime - GetTickCount64() * 10000;

    FILETIME bootFileTime;
    bootFileTime.dwHighDateTime = bootTime >> 32;
    bootFileTime.dwLowDateTime = (DWORD)bootTime;

    FileTimeToSystemTime(&bootFileTime, &st);

    printf("System Boot Time: %02d/%02d/%d %02d:%02d:%02d\n",
           st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);
}

// Helper function to initialize COM and set security levels
HRESULT InitializeCOMSecurity() {
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        printf("Failed to initialize COM library. Error code = 0x%X\n", hres);
        return hres;
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );
    if (FAILED(hres)) {
        printf("Failed to initialize security. Error code = 0x%X\n", hres);
        CoUninitialize();
        return hres;
    }

    return S_OK;
}

// Helper function to create and connect to WMI namespace
HRESULT CreateWMIConnection(IWbemLocator **pLoc, IWbemServices **pSvc) {
    HRESULT hres;

    hres = CoCreateInstance(
        &CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        &IID_IWbemLocator,
        (LPVOID *)pLoc
    );
    if (FAILED(hres)) {
        printf("Failed to create IWbemLocator object. Error code = 0x%X\n", hres);
        CoUninitialize();
        return hres;
    }

    hres = (*pLoc)->lpVtbl->ConnectServer(
        *pLoc,
        L"ROOT\\CIMV2",
        NULL,
        NULL,
        0,
        0,
        0,
        NULL,
        pSvc
    );
    if (FAILED(hres)) {
        printf("Could not connect to WMI namespace. Error code = 0x%X\n", hres);
        (*pLoc)->lpVtbl->Release(*pLoc);
        CoUninitialize();
        return hres;
    }

    hres = CoSetProxyBlanket(
        (IUnknown *)*pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hres)) {
        printf("Could not set proxy blanket. Error code = 0x%X\n", hres);
        (*pSvc)->lpVtbl->Release(*pSvc);
        (*pLoc)->lpVtbl->Release(*pLoc);
        CoUninitialize();
        return hres;
    }

    return S_OK;
}

// Helper function to get WMI property value as a string
void GetWMIPropertyString(IWbemClassObject *pclsObj, BSTR propertyName, char *buffer, size_t bufferSize) {
    VARIANT vtProp;
    VariantInit(&vtProp);

    HRESULT hr = pclsObj->lpVtbl->Get(pclsObj, propertyName, 0, &vtProp, 0, 0);
    if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR) {
        wcstombs(buffer, vtProp.bstrVal, bufferSize);
    }

    VariantClear(&vtProp);
}

// Function to get WMI system information
void getWMISystemInfo() {
    HRESULT hres;
    IWbemLocator *pLoc = NULL;
    IWbemServices *pSvc = NULL;

    hres = InitializeCOMSecurity();
    if (FAILED(hres)) return;

    hres = CreateWMIConnection(&pLoc, &pSvc);
    if (FAILED(hres)) return;

    IEnumWbemClassObject *pEnumerator = NULL;
    hres = pSvc->lpVtbl->ExecQuery(
        pSvc,
        L"WQL",
        L"SELECT * FROM Win32_OperatingSystem",
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
    );
    if (FAILED(hres)) {
        printf("Query for operating system name failed. Error code = 0x%X\n", hres);
        pSvc->lpVtbl->Release(pSvc);
        pLoc->lpVtbl->Release(pLoc);
        CoUninitialize();
        return;
    }

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->lpVtbl->Next(pEnumerator, WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) break;

        char buffer[256];

        GetWMIPropertyString(pclsObj, L"RegisteredUser", buffer, sizeof(buffer));
        printf("Registered Owner: %s\n", buffer);

        GetWMIPropertyString(pclsObj, L"Organization", buffer, sizeof(buffer));
        printf("Registered Organization: %s\n", buffer);

        GetWMIPropertyString(pclsObj, L"SerialNumber", buffer, sizeof(buffer));
        printf("Product ID: %s\n", buffer);

        pclsObj->lpVtbl->Release(pclsObj);
    }

    pSvc->lpVtbl->Release(pSvc);
    pLoc->lpVtbl->Release(pLoc);
    pEnumerator->lpVtbl->Release(pEnumerator);
    CoUninitialize();
}

int main() {
    printf("System Information:\n\n");

    getOSInfo();
    printf("\n");

    getSystemInfo();
    printf("\n");

    getMemoryInfo();
    printf("\n");

    getProcessorInfo();
    printf("\n");

    getBootTime();
    printf("\n");

    getWMISystemInfo();
    printf("\n");

    return 0;
}
