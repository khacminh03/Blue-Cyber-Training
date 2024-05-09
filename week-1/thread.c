#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <windows.h>
#include <tlhelp32.h>
#include <sysinfoapi.h>
#include <time.h>

double result[3][2] = {};
void swap(double arr[][2], int i, int j) {
    double temp[2];
    for (int k = 0; k < 2; k++) {
        temp[k] = arr[i][k];
        arr[i][k] = arr[j][k];
        arr[j][k] = temp[k];
    }
}


void bubbleSort(double arr[][2], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j][1] > arr[j + 1][1]) {
                swap(arr, j, j + 1);
            }
        }
    }
}
void* countCharacter(void *arg) {
    clock_t start = clock();
    FILE *file;
    file = fopen("1.txt", "r");
    char wordList[100];
    int count = 0;
    fgets(wordList, 100, file);
    printf("%s\n", wordList);
    for (int i = 0; i < strlen(wordList); i++) {
        if (wordList[i] == ' ' && wordList[i + 1] != ' ') {
            count += 1;
        }
    }
    count += 1;
    printf("Found total %d in file 1.txt.\n", count);

    double elapsedTime = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    printf("Thread count character in a file complete in %f.\n", elapsedTime);
    result[0][0] = 1;
    result[0][1] = elapsedTime;
    pthread_exit(NULL);
}

void* countCurrentFile(void* arg) {
    clock_t start = clock();
    int count = 0;
    DIR *dirp;
    struct dirent *entry;
    struct stat statbuf;

    dirp = opendir(".");
    if (dirp == NULL) {
        perror("Error opening directory");
        return NULL;
    }

    while ((entry = readdir(dirp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; 
        }
        if (stat(entry->d_name, &statbuf) == -1) {
            perror("Error reading file information");
            continue;
        }
        if (S_ISREG(statbuf.st_mode)) { 
            count++;
        }
    }

    closedir(dirp);
    printf("Current directory has %d files.\n", count);

    double elapsedTime = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    printf("Thread count list of file current directory complete in %f.\n", elapsedTime);
    result[1][0] = 2;
    result[1][1] = elapsedTime;
    pthread_exit(NULL);
}

void* countThread(void* arg) {
    clock_t start = clock();
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    PROCESSENTRY32 pe32;
    DWORD dwThreadCount = 0;

    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE) {
        printf("CreateToolhelp32Snapshot failed.\n");
        return NULL;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hThreadSnap, &pe32)) {
        printf("Process32First failed.\n");
        CloseHandle(hThreadSnap);
        return NULL;
    }

    do {
        if (strcmp(pe32.szExeFile, "explorer.exe") == 0) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
            if (hProcess != NULL) {
                HANDLE hThreadSnapExplorer = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                if (hThreadSnapExplorer != INVALID_HANDLE_VALUE) {
                    THREADENTRY32 te32;
                    te32.dwSize = sizeof(THREADENTRY32);

                    if (Thread32First(hThreadSnapExplorer, &te32)) {
                        do {
                            if (te32.th32OwnerProcessID == pe32.th32ProcessID) {
                                dwThreadCount++;
                            }
                        } while (Thread32Next(hThreadSnapExplorer, &te32));
                    }

                    CloseHandle(hThreadSnapExplorer);
                }

                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hThreadSnap, &pe32));

    CloseHandle(hThreadSnap);

    printf("Number of threads in explorer.exe: %d\n", dwThreadCount);
    double elapsedTime = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    printf("Thread count thread of explorer.exe complete in %f.\n", elapsedTime);
    result[2][0] = 3;
    result[2][1] = elapsedTime;
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[3];
    pthread_create(&tid[0], NULL, countCharacter, NULL);
    pthread_create(&tid[1], NULL, countCurrentFile, NULL);
    pthread_create(&tid[2], NULL, countThread, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }
    int n = sizeof(result) / sizeof(result[0]);
    bubbleSort(result, n);
    for (int i = 0; i < n; i++) {
        printf("%d prize: Thread %f\n", i + 1, result[i][0]);
    }
}
