#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <utility>

class Fraction
{
public:
    // constructors
    Fraction();
    Fraction(int numerator);
    Fraction(int numerator, int denominator);

    void setDenominator(long a);
    void setNumerator(long a);
    long getDenominator() const;
    long getNumerator() const;

    std::size_t getTotalSymbols() const;
    void Simplify();
    friend Fraction FractionGcd(Fraction& a, Fraction& b);

    // operators
    bool operator<(const Fraction& a) const;
    bool operator>(const Fraction& a) const;
    bool operator==(const Fraction& a) const;
    bool operator!=(const Fraction& a) const;

    Fraction operator+(const Fraction& a) const;
    Fraction operator-(const Fraction& a) const;
    Fraction operator*(const Fraction& a) const;
    Fraction operator/(const Fraction& a) const;

    Fraction& operator+=(const Fraction& a);
    Fraction& operator-=(const Fraction& a);
    Fraction& operator*=(const Fraction& a);
    Fraction& operator/=(const Fraction& a);

    friend std::ostream& operator<< (std::ostream &out, const Fraction &a);
    friend std::istream& operator>> (std::istream& in, Fraction& a);

private:
    void CheckDivisionByZero();
    friend std::pair<Fraction, Fraction> CommonDenominator
        (const Fraction& a, const Fraction& b);

private:
    long numerator_;
    long denominator_;
};

#endif