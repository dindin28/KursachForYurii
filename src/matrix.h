#ifndef MATRIX_H
#define MATRIX_H

#include "fraction.h"

#include <vector>
#include <initializer_list>

class Matrix
{
public:
    Matrix();
    Matrix(std::size_t, std::size_t);
    Matrix(std::initializer_list<std::initializer_list<Fraction>> v);

    // Methods
    std::size_t getRowsCount() const;
    std::size_t getColumnsCount() const;

    void Simplify();
    Fraction& at(std::size_t x, std::size_t y);

    std::size_t rang() const;
    Fraction determinant() const;

    std::vector<Fraction> gauss(bool isVerbose = false) const;
    std::vector<Fraction> cramera(bool isVerbose = false) const;
    std::vector<Fraction> montane(bool isVerbose = false) const;
    std::vector<Fraction> matrixMethod(bool isVerbose = false) const;

    // operators
    friend Matrix operator*(const Fraction& fr, const Matrix& matrix);
    friend std::vector<Fraction> operator*(const Matrix& matrix, const std::vector<Fraction>& vec);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& a);
    friend std::istream& operator>>(std::istream& in, Matrix& a);

private:
    // matrix_method_helper
    Matrix algebraicComplement() const;
    Matrix algebraicComplementHelper(std::pair<std::size_t, std::size_t> xy_coord) const;
    Matrix transpon() const;

    // cramera_helper
    Matrix getMatrixForCramera(std::size_t column) const;

    // row_helpers
    void swapRows(std::size_t row1, std::size_t row2);
    std::size_t rowsWithZeros() const;

    // column_helpers
    std::size_t getMaxSymbolCountInColumn(std::size_t column);
    std::vector<Fraction> getColumnVector(std::size_t column) const;

    // ios_helpers
    std::ostream& show(std::ostream& out, std::pair<std::size_t, std::size_t> coord);
    std::ostream& show(std::ostream& out);
    std::istream& input(std::istream& in);

private:
    std::vector<std::vector<Fraction>> matrix_;
    bool is_expand_;
};

#endif // MATRIX_H