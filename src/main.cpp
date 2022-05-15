#include <iostream>
#include <algorithm>
#include "utilities.h"
#include "fraction.h"
#include "matrix.h"

using namespace std;

int main()
{
    Matrix matrix { 
        {Fraction(2), Fraction(2, 98), Fraction(3), Fraction(1)},
        {Fraction(3,5), Fraction(5, 8), Fraction(6), Fraction(1)},
        {Fraction(5), Fraction(-5, 8), Fraction(7), Fraction(1)}};

    matrix.Simplify();

    std::cout << matrix << std::endl;
    auto p = matrix.gauss(true);
    for(const auto& iter : p)
    {
        std::cout << iter << ' ';
    }
    std::cout << std::endl;
    auto cramera = matrix.cramera(true);
    for(const auto& iter : cramera)
    {
        std::cout << iter << ' ';
    }
    std::cout << std::endl;
    return 0;
}