#include "utilities.h"

#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#endif // __linux__

void EditMatrix(Matrix& matrix)
{
    int ch = -1;
    bool isWrongInput;
    while(ch != 0)
    {
        isWrongInput = false;
        do
        {
            clear();
            std::cout << "Matrix:\n" << matrix << "\n";
            if(isWrongInput)
            { std::cout << "Wrong input!\n"; }
            else
            { isWrongInput = true; }
            std::cout << "1) Calculate determinant\n";
            std::cout << "2) Calculate matrix rang\n";
            std::cout << "3) Calculate matrix via gauss\n";
            std::cout << "4) Calculate matrix via gauss(verbose)\n";
            std::cout << "5) Calculate matrix via cramera\n";
            std::cout << "6) Calculate matrix via cramera(verbose)\n";
            std::cout << "7) Calculate matrix via matrix method\n";
            std::cout << "8) Calculate matrix via matrix method(verbose)\n";
            std::cout << "9) Enter new matrix\n";
            std::cout << "0) Exit\n\n";
            std::cout << "Enter number: ";
            std::cin >> ch;
        } while (ch != 0 && ch != 1 && ch != 2 &&
                 ch != 3 && ch != 4 && ch != 5 &&
                 ch != 6 && ch != 7 && ch != 8 &&
                 ch != 9);
        try
        {
            switch (ch)
            {
            case (1):
                {
                    std::cout << "Determinant: " << matrix.determinant() << std::endl;
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (2):
                {
                    std::cout << "Rang: " << matrix.rang() << std::endl;
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (3):
                {
                    auto answers = matrix.gauss();
                    std::cout << "Answers (gauss method): " << std::endl;
                    PrintAnswers(answers);
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (4):
                {
                    std::cout << "Action course:" << std::endl;
                    auto answers = matrix.gauss(true);
                    std::cout << "Answers (gauss method): " << std::endl;
                    PrintAnswers(answers);
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (5):
                {
                    auto answers = matrix.cramera();
                    std::cout << "Answers (cramera method): " << std::endl;
                    PrintAnswers(answers);
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (6):
                {
                    std::cout << "Action course:" << std::endl;
                    auto answers = matrix.cramera(true);
                    std::cout << "Answers (cramera method): " << std::endl;
                    PrintAnswers(answers);
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (7):
                {
                    auto answers = matrix.matrixMethod();
                    std::cout << "Answers (matrix method method): " << std::endl;
                    PrintAnswers(answers);
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (8):
                {
                    std::cout << "Action course:" << std::endl;
                    auto answers = matrix.matrixMethod(true);
                    std::cout << "Answers (matrix method method): " << std::endl;
                    PrintAnswers(answers);
                    std::cout << "Press any key";
                    _getch();
                }
                break;
            case (9):
                {
                    std::cin >> matrix;
                }
                break;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl
                      << "Try another method or change matrix" << std::endl
                      << "Press any key";
            _getch();
        }
    }
}

void PrintAnswers(const std::vector<Fraction> ans)
{
    for(std::size_t i = 0; i < ans.size(); ++i)
    {
        std::cout << "x[" << i + 1 << "] = " << ans.at(i) << std::endl;
    }
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