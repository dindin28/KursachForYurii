#include "matrix.h"

#include "utilities.h"

#include <algorithm>
#include <iomanip>
#include <cmath>
#include <sstream>

//=================================
// external_functions_declarations
//=================================
void makeSpace(std::ostream &out, std::size_t spaces);

//=================================
// constructors
//=================================
Matrix::Matrix(std::initializer_list<std::initializer_list<Fraction>> v)
    : matrix_(v.begin(), v.end()), is_expand_(true)
{
}

Matrix::Matrix()
    : is_expand_(true)
{}


Matrix::Matrix(std::size_t rows_count, std::size_t columns_count)
    : is_expand_(false)
{
    for(std::size_t i = 0; i < rows_count; ++i)
    {
        matrix_.push_back(std::vector<Fraction>(columns_count));
    }
}

//=================================
// operators
//=================================

Matrix operator*(const Fraction& fr, const Matrix& matrix)
{
    Matrix new_matrix(matrix);
    for(auto& rows : new_matrix.matrix_)
    {
        for(auto& iter : rows)
        {
            iter *= fr;
        }
    }
    return new_matrix;
}

std::vector<Fraction> operator*(const Matrix& matrix, const std::vector<Fraction>& vec)
{
    std::vector<Fraction> return_vector;
    for(std::size_t y = 0; y < matrix.getRowsCount(); ++y)
    {
        Fraction sum(0);
        for(std::size_t x = 0; x < matrix.getColumnsCount(); ++x)
        {
            sum += const_cast<Matrix&>(matrix).at(x, y) * vec.at(x);
        }
        return_vector.push_back(sum);
    }
    return return_vector;
}

std::ostream &operator<<(std::ostream &out, const Matrix &a)
{
    return const_cast<Matrix &>(a).show(out);
}

std::istream &operator>>(std::istream &in, Matrix &a)
{
    return a.input(in);
}

//=================================
// public_methods
//=================================
std::size_t Matrix::getRowsCount() const
{
    return matrix_.size();
}

std::size_t Matrix::getColumnsCount() const
{
    return matrix_.at(0).size();
}

void Matrix::Simplify()
{
    for (auto &rows : matrix_)
        for (auto &iter : rows)
            iter.Simplify();
}

std::size_t Matrix::rang() const
{
    return getRowsCount() - rowsWithZeros();
}

Fraction Matrix::determinant() const
{
    Matrix new_matrix(*this);
    Fraction supElement(1, 1);
    for (std::size_t k = 0; k < getRowsCount(); ++k)
    {
        for (std::size_t i = 0; i < getRowsCount(); ++i)
        {
            if (i != k)
            {
                for (std::size_t j = getColumnsCount() - 1; j > 0; --j)
                {
                    Fraction firPro(new_matrix.at(k, k) * new_matrix.at(j, i));
                    Fraction secPro(new_matrix.at(j, k) * new_matrix.at(k, i));
                    Fraction diff(firPro - secPro);
                    new_matrix.at(j, i) = diff / supElement;
                }
            }
        }
        supElement = new_matrix.at(k, k);
    }
    return supElement;
}

std::vector<Fraction> Matrix::gauss(bool isVerbose) const
{
    Matrix new_matrix(*this);

    const std::size_t row_count = getRowsCount();
    const std::size_t column_count = getColumnsCount() - 1;

    std::vector<std::size_t> where(row_count, -1);
    for (std::size_t column = 0; column < column_count; ++column)
    {
        std::size_t row = column;
        std::size_t sel = row;
        for (std::size_t i = row; i < row_count; ++i)
            if (new_matrix.at(column, i) > new_matrix.at(column, sel))
                sel = i;

        new_matrix.swapRows(sel, row);
        where[column] = row;

        if (isVerbose)
        {
            std::cout << new_matrix << std::endl;
        }

        for (std::size_t i = 0; i < row_count; ++i)
            if (i != row)
            {
                Fraction divider = new_matrix.at(column, i) / new_matrix.at(column, row);
                for (std::size_t j = column; j <= column_count; ++j)
                    new_matrix.at(j, i) -= new_matrix.at(j, row) * divider;
            }
        if (isVerbose)
        {
            std::cout << new_matrix << std::endl;
        }
    }

    std::vector<Fraction> ans(column_count);
    for (std::size_t i = 0; i < column_count; ++i)
        if (where[i] != -1)
            ans[i] = new_matrix.at(column_count, where[i]) / new_matrix.at(i, where[i]);

    return ans;
}

std::vector<Fraction> Matrix::cramera(bool isVerbose) const
{
    std::size_t size = getColumnsCount() - 1;
    std::vector<Fraction> ans(size);
    Fraction mainDeterminant = this->determinant();
    if (isVerbose)
    {
        std::cout << "det = " << mainDeterminant << std::endl;
    }
    for (std::size_t i = 0; i < size; i++)
    {
        Fraction buff(mainDeterminant);
        Matrix buffMatrix(this->getMatrixForCramera(i));
        auto det = buffMatrix.determinant();
        if (isVerbose)
        {
            std::cout << "det" << i + 1 << " = " << det << std::endl;
        }
        ans[i] = det / buff;
    }

    return ans;
}

std::vector<Fraction> Matrix::matrixMethod(bool isVerbose) const
{
    if(determinant() != Fraction(0))
    {
        auto matrix = algebraicComplement();
        if(isVerbose)
        {
            std::cout << "Algebraic complement matrix:\n" << matrix << std::endl;
        }
        matrix = matrix.transpon();
        if(isVerbose)
        {
            std::cout << "Transponation matrix:\n" << matrix << std::endl;
        }
        matrix = (Fraction(1) / this->determinant()) * matrix;
        if(isVerbose)
        {
            std::cout << "matrix^(-1):\n" << matrix << std::endl;
        }
        auto vec = matrix * getColumnVector(getColumnsCount() - 1);

        return vec;
    }
    else
    {
        std::cout << "Determinant is zero\n";
        return std::vector<Fraction>();
    }
}

//=================================
// private_methods
//=================================
Matrix Matrix::algebraicComplement() const
{
    Matrix return_matrix(getRowsCount(), getRowsCount());
    for(std::size_t y = 0; y < return_matrix.getRowsCount(); ++y)
    {
        for(std::size_t x = 0; x < return_matrix.getColumnsCount(); ++x)
        {
            return_matrix.at(x, y) =
                Fraction(std::pow(-1, x + y)) * algebraicComplementHelper(std::make_pair(x, y)).determinant();
        }
    }
    return return_matrix;
}

Matrix Matrix::algebraicComplementHelper(std::pair<std::size_t, std::size_t> xy_coord) const
{
    Matrix return_matrix(getRowsCount() - 1, getRowsCount() - 1);
    for(std::size_t y_main = 0, y_new = 0; y_main < getRowsCount(); ++y_main)
    {
        if(y_main != xy_coord.second)
        {
            for(std::size_t x_main = 0, x_new = 0; x_main < getRowsCount(); ++x_main)
            {
                if(x_main != xy_coord.first)
                {                    
                    return_matrix.at(x_new, y_new) = const_cast<Matrix*>(this)->at(x_main, y_main);
                    ++x_new;
                }
            }
            ++y_new;
        }
    }
    return return_matrix;
}

Matrix Matrix::transpon() const
{
    Matrix new_matrix(*this);
    for(std::size_t y = 0; y < getRowsCount(); ++y)
    {
        for(std::size_t x = y + 1; x < getColumnsCount(); ++x)
        {
            std::swap(new_matrix.at(x, y), new_matrix.at(y, x));
        }
    }
    return new_matrix;
}

Matrix Matrix::getMatrixForCramera(std::size_t column) const
{
    Matrix newMatrix(*this);
    for (std::size_t i = 0; i < getRowsCount(); ++i)
    {
        for (std::size_t j = 0; j < getColumnsCount() - 1; ++j)
        {
            if (j == column)
            {
                newMatrix.at(j, i) = const_cast<Matrix *>(this)->at(getColumnsCount() - 1, i);
            }
            else
            {
                newMatrix.at(j, i) = const_cast<Matrix *>(this)->at(j, i);
            }
        }
    }
    return newMatrix;
}

void Matrix::swapRows(std::size_t row1, std::size_t row2)
{
    if (row1 != row2)
    {
        for (std::size_t i = 0; i < getColumnsCount(); ++i)
        {
            std::swap(at(i, row1), at(i, row2));
        }
    }
}

std::size_t Matrix::rowsWithZeros() const
{
    std::size_t sum = 0;
    for (std::size_t i = 0; i < getRowsCount(); i++)
    {
        for (std::size_t j = 0; j < getColumnsCount(); j++)
        {
            if (const_cast<Matrix *>(this)->at(j, i) != Fraction(0, 1))
                break;
            if (const_cast<Matrix *>(this)->at(j, i) == Fraction(0, 1) && j == getColumnsCount() - 1)
                sum++;
        }
    }
    return sum;
}

Fraction &Matrix::at(std::size_t x, std::size_t y)
{
    return std::ref(matrix_.at(y).at(x));
}

std::size_t Matrix::getMaxSymbolCountInColumn(std::size_t column)
{
    std::vector<Fraction> column_vec(getColumnVector(column));
    auto p = std::max_element(column_vec.begin(), column_vec.end(), [](auto l, auto r)
                              { return l.getTotalSymbols() < r.getTotalSymbols(); });
    return (*p).getTotalSymbols();
}

std::vector<Fraction> Matrix::getColumnVector(std::size_t column) const
{
    std::vector<Fraction> return_vector;
    for (std::size_t i = 0; i < getRowsCount(); ++i)
    {
        return_vector.push_back(const_cast<Matrix *>(this)->at(column, i));
    }
    return return_vector;
}

//===========================
// ios_helpers
//===========================
std::ostream &Matrix::show(std::ostream &out, std::pair<std::size_t, std::size_t> coord)
{
    std::vector<std::size_t> max_symbols(getColumnsCount());
    for (std::size_t i = 0; i < getColumnsCount(); ++i)
    {
        max_symbols[i] = getMaxSymbolCountInColumn(i);
    }
    std::stringstream str;
    for (std::size_t i = 0; i < getRowsCount(); ++i)
    {
        for (std::size_t j = 0; j < getColumnsCount(); ++j)
        {
            makeSpace(str, (max_symbols[j] + 1) - at(j, i).getTotalSymbols());
            (j == coord.first && i == coord.second) ? (str << 'X') : (str << at(j, i));
            if (j == getColumnsCount() - 2 && is_expand_)
            {
                str << " |";
            }
        }
        str << std::endl;
    }
    return out << str.rdbuf();
}

std::ostream &Matrix::show(std::ostream &out)
{
    std::vector<std::size_t> max_symbols(getColumnsCount());
    for (std::size_t i = 0; i < getColumnsCount(); ++i)
    {
        max_symbols[i] = getMaxSymbolCountInColumn(i);
    }
    std::stringstream str;
    for (std::size_t i = 0; i < getRowsCount(); ++i)
    {
        for (std::size_t j = 0; j < getColumnsCount(); ++j)
        {
            makeSpace(str, (max_symbols[j] + 1) - at(j, i).getTotalSymbols());
            str << at(j, i);
            if (j == getColumnsCount() - 2 && is_expand_)
            {
                str << " |";
            }
        }
        str << std::endl;
    }
    return out << str.rdbuf();
}

std::istream &Matrix::input(std::istream &in)
{
    std::size_t rows, columns;
    bool isWrongInput = false;
    do
    {
        clear();
        if (isWrongInput)
        {
            std::cout << "Wrong format! ";
        }
        else
        {
            isWrongInput = true;
        }
        std::cout << "Enter count of rows, columns (rows == columns - 1): ";
        in >> rows >> columns;
    } while (rows + 1 != columns || rows <= 1);
    matrix_.clear();
    matrix_.resize(rows);
    for (auto &iter : matrix_)
    {
        iter.resize(columns);
    }

    for (std::size_t i = 0; i < rows; ++i)
    {
        for (std::size_t j = 0; j < columns; ++j)
        {
            clear();
            show(std::cout, std::make_pair(j, i));
            Fraction f;
            in >> f;
            at(j, i) = f;
        }
    }
    this->Simplify();
    return in;
}

//===========================
// external_functions
//===========================
void makeSpace(std::ostream &out, std::size_t spaces)
{
    for (std::size_t i = 0; i < spaces; ++i)
    {
        out << " ";
    }
}
