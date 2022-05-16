#ifndef UTILITIES_H
#define UTILITIES_H

#include "matrix.h"

void clear();
void EditMatrix(Matrix& matrix);
void PrintAnswers(const std::vector<Fraction> ans);

#ifdef __linux__
char _getch(void);
#elif _WIN32
#include <conio.h>
#endif

#endif