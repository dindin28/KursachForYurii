#ifndef UTILITIES_H
#define UTILITIES_H

int digitsOfNumber(int x);
void clear();

#ifdef __linux__
char _getch(void);
#elif _WIN32
#include <conio.h>
#endif

#endif