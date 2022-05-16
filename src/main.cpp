#include <iostream>
#include <algorithm>
#include "utilities.h"
#include "fraction.h"
#include "matrix.h"

using namespace std;

int main()
{
    Matrix matrix;
    std::cin >> matrix;
    EditMatrix(matrix);
    return 0;
}