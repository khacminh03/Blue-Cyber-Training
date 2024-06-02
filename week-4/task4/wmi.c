#include <windows.h>
#include <wbemidl.h>
#include <psapi.h>
#include <stdio.h>
#include <stdbool.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "psapi.lib")

HRESULT STDMETHODCALLTYPE MyIndicate(
    IWbemObjectSink *This,
    LONG lObjectCount,
    IWbemClassObject **apObjArray
) {
    VARIANT vtProp;
    for (LONG i = 0; i < lObjectCount; i++) {
        HRESULT hr = apObjArray[i]->lpVtbl->Get(apObjArray[i], L"ProcessId", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            DWORD processID = vtProp.uintVal;
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
            if (hProcess) {
                wchar_t processName[MAX_PATH];
                if (GetModuleBaseNameW(hProcess, NULL, processName, sizeof(processName) / sizeof(wchar_t))) {
                    if (wcscmp(processName, L"Calculator.exe") == 0) {
                        // Run Notepad
                        STARTUPINFOW si = { sizeof(STARTUPINFOW) };
                        PROCESS_INFORMATION pi;
                        if (CreateProcessW(L"C:\\Windows\\System32\\notepad.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                            CloseHandle(pi.hProcess);
                            CloseHandle(pi.hThread);
                        }
                    }
                }
                CloseHandle(hProcess);
            }
        }
        VariantClear(&vtProp);
    }
    return WBEM_S_NO_ERROR;
}

HRESULT STDMETHODCALLTYPE MySetStatus(
    IWbemObjectSink *This,
    LONG lFlags,
    HRESULT hResult,
    BSTR strParam,
    IWbemClassObject *pObjParam
) {
    return WBEM_S_NO_ERROR;
}

typedef struct {
    IWbemObjectSinkVtbl *lpVtbl;
    LONG refCount;
} WbemObjectSink;

ULONG STDMETHODCALLTYPE AddRef(IWbemObjectSink *This) {
    WbemObjectSink *sink = (WbemObjectSink*)This;
    return InterlockedIncrement(&sink->refCount);
}

ULONG STDMETHODCALLTYPE Release(IWbemObjectSink *This) {
    WbemObjectSink *sink = (WbemObjectSink*)This;
    ULONG refCount = InterlockedDecrement(&sink->refCount);
    if (refCount == 0) {
        free(sink);
    }
    return refCount;
}

HRESULT STDMETHODCALLTYPE QueryInterface(IWbemObjectSink *This, REFIID riid, void **ppv) {
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IWbemObjectSink)) {
        *ppv = This;
        AddRef(This);
        return S_OK;
    } else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
}

IWbemObjectSinkVtbl g_WbemObjectSinkVtbl = {
    QueryInterface,
    AddRef,
    Release,
    MyIndicate,
    MySetStatus
};

int main() {
    HRESULT hr;
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        printf("Failed to initialize COM library. Error code = 0x%x\n", hr);
        return 1;
    }

    hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
                              RPC_C_AUTHN_LEVEL_DEFAULT,
                              RPC_C_IMP_LEVEL_IMPERSONATE,
                              NULL, EOAC_NONE, NULL);
    if (FAILED(hr)) {
        printf("Failed to initialize security. Error code = 0x%x\n", hr);
        CoUninitialize();
        return 1;
    }

    IWbemLocator *pLoc = NULL;
    hr = CoCreateInstance(&CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                          &IID_IWbemLocator, (LPVOID *)&pLoc);
    if (FAILED(hr)) {
        printf("Failed to create IWbemLocator object. Error code = 0x%x\n", hr);
        CoUninitialize();
        return 1;
    }

    IWbemServices *pSvc = NULL;
    hr = pLoc->lpVtbl->ConnectServer(
        pLoc,
        L"ROOT\\CIMV2",
        NULL,
        NULL,
        0,
        0,
        0,
        0,
        &pSvc
    );
    if (FAILED(hr)) {
        printf("Could not connect. Error code = 0x%x\n", hr);
        pLoc->lpVtbl->Release(pLoc);
        CoUninitialize();
        return 1;
    }

    hr = CoSetProxyBlanket(
        (IUnknown *)pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hr)) {
        printf("Could not set proxy blanket. Error code = 0x%x\n", hr);
        pSvc->lpVtbl->Release(pSvc);
        pLoc->lpVtbl->Release(pLoc);
        CoUninitialize();
        return 1;
    }

    WbemObjectSink *pStubSink = (WbemObjectSink*)malloc(sizeof(WbemObjectSink));
    pStubSink->lpVtbl = &g_WbemObjectSinkVtbl;
    pStubSink->refCount = 1;

    hr = pSvc->lpVtbl->ExecNotificationQueryAsync(
        pSvc,
        L"WQL",
        L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'",
        WBEM_FLAG_SEND_STATUS,
        NULL,
        (IWbemObjectSink *)pStubSink
    );
    if (FAILED(hr)) {
        printf("ExecNotificationQueryAsync failed. Error code = 0x%x\n", hr);
        pStubSink->lpVtbl->Release((IWbemObjectSink*)pStubSink);
        pSvc->lpVtbl->Release(pSvc);
        pLoc->lpVtbl->Release(pLoc);
        CoUninitialize();
        return 1;
    }

    printf("Waiting for events...\n");

    while (true) {
        Sleep(1000);
    }

    pSvc->lpVtbl->CancelAsyncCall(pSvc, (IWbemObjectSink*)pStubSink);
    pStubSink->lpVtbl->Release((IWbemObjectSink*)pStubSink);
    pSvc->lpVtbl->Release(pSvc);
    pLoc->lpVtbl->Release(pLoc);
    CoUninitialize();

    return 0;
}
