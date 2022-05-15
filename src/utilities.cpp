#include "utilities.h"

#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#endif // __linux__

int digitsOfNumber(int x) {
    x = abs(x);
    int n = 1;
    while ((x /= 10) > 0)
        n++;
    return n;
}

void clear()
{
#ifdef __linux__
    std::system("clear");
#elif _WIN32
    std::system("cls");
#endif
}

#ifdef __linux__
char _getch(void)
{
    char buf = 0;
    struct termios old = { 0 };
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    //old.c_cc[VGoodIME] = 0;
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    printf("%c\n", buf);
    return buf;
}
#endif // __linux__