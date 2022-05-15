#include "fraction.h"
#include "utilities.h"

#include <cmath>
#include <iostream>
#include <string>
#include <numeric>
#include <sstream>
#include <algorithm>

using namespace std;

Fraction::Fraction()
    : numerator_(0), denominator_(1)
{}

Fraction::Fraction(int numerator)
    : numerator_(numerator), denominator_(1)
{}

Fraction::Fraction(int numerator, int denominator)
    : numerator_(numerator), denominator_(denominator)
{ CheckDivisionByZero(); }

long Fraction::getDenominator() const { return denominator_; }
void Fraction::setDenominator(long a) { denominator_ = a; }
long Fraction::getNumerator() const { return numerator_; }
void Fraction::setNumerator(long a) { numerator_ = a; }

std::size_t Fraction::getTotalSymbols() const
{
    stringstream str;
    str << *this;
    return str.str().size();
}

bool Fraction::operator<(const Fraction& a) const
{
    auto fr_pair = CommonDenominator(*this, a);
    return fr_pair.first.getNumerator() < fr_pair.second.getNumerator();
}

bool Fraction::operator>(const Fraction& a) const
{
    auto fr_pair = CommonDenominator(*this, a);
    return fr_pair.first.getNumerator() > fr_pair.second.getNumerator();
}

bool Fraction::operator==(const Fraction& a) const
{
    auto fr_pair = CommonDenominator(*this, a);
    return fr_pair.first.getNumerator() == fr_pair.second.getNumerator();
}

bool Fraction::operator!=(const Fraction& a) const
{ return !(*this == a); }

Fraction Fraction::operator+(const Fraction& a) const
{
    auto fr_pair = CommonDenominator(*this, a);

    long numerator_ = fr_pair.first.getNumerator() + fr_pair.second.getNumerator();
    long denominator_ = fr_pair.first.getDenominator();

    Fraction return_fraction(numerator_, denominator_);

    return_fraction.Simplify();
    return return_fraction;
}

Fraction Fraction::operator-(const Fraction& a) const
{
    auto fr_pair = CommonDenominator(*this, a);

    long numerator_ = fr_pair.first.getNumerator() - fr_pair.second.getNumerator();
    long denominator_ = fr_pair.first.getDenominator();

    Fraction return_fraction(numerator_, denominator_);

    return_fraction.Simplify();
    return return_fraction;
}

Fraction Fraction::operator*(const Fraction& a) const
{
    Fraction return_fraction(numerator_ * a.getNumerator(), denominator_ * a.getDenominator());

    return_fraction.Simplify();
    return return_fraction;
}

Fraction Fraction::operator/(const Fraction& a) const
{
    if(a.getNumerator() == 0 || denominator_ == 0)
    {
        stringstream str;
        str << "Fraction error: Attempted " << *this << " divide by " << a;
        throw runtime_error(str.str());
    }
    Fraction reverse_a(a.getDenominator(), a.getNumerator());
    Fraction return_fraction(getNumerator(), getDenominator());
    return_fraction = return_fraction * reverse_a;
    return_fraction.Simplify();
    return return_fraction;
}

Fraction& Fraction::operator+=(const Fraction& a)
{ return *this = *this + a; }

Fraction& Fraction::operator-=(const Fraction& a)
{ return *this = *this - a; }

Fraction& Fraction::operator*=(const Fraction& a)
{ return *this = *this * a; }

Fraction& Fraction::operator/=(const Fraction& a)
{ return *this = *this / a; }

std::ostream& operator<< (std::ostream &out, const Fraction &a)
{
    std::stringstream str;
    str << a.numerator_;
    if(a.denominator_ != 1)
    { str << "/" << a.denominator_; }

    return out << str.rdbuf();
}

std::istream& operator>> (std::istream& in, Fraction& a)
{
    string strBuff;
    bool isWrongInput = false;
    do
    {
        if(isWrongInput)
        { cout << "Wrong format! "; }
        else
        { isWrongInput = true; }

        cout << "Enter fraction (num/den or num): ";
        in >> strBuff;
    } while(strBuff.find_first_not_of("1234567890/-") != string::npos ||
            count(strBuff.begin(), strBuff.end(), '/') > 1 ||
            count(strBuff.begin(), strBuff.end(), '-') > 1);

    if (strBuff.find_first_of("/") == string::npos)
    {
        a.numerator_ = stoi(strBuff.substr(0, strBuff.length()));
        a.denominator_ = 1;
    }
    else
    {
        a.numerator_ = stoi(strBuff.substr(0, strBuff.find_first_of("/")));
        a.denominator_ = stoi(strBuff.substr(strBuff.find_first_of("/") + 1, strBuff.length()));
    }

    a.Simplify();
    return in;
}

Fraction FractionGcd(Fraction& a, Fraction& b)
{
    long comDen = lcm(a.getDenominator(), b.getDenominator());
    int firNum = a.getNumerator() * (comDen / a.getDenominator());
    int secNum = b.getNumerator() * (comDen / b.getDenominator());
    return Fraction(gcd(firNum, secNum), comDen);
}

void Fraction::Simplify()
{
    if(denominator_ < 0)
    {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }
    auto gcd = std::gcd(abs(numerator_), abs(denominator_));
    numerator_ /= gcd;
    denominator_ /= gcd;
}

void Fraction::CheckDivisionByZero()
{
    if(denominator_ == 0)
    {
        stringstream str;
        str << "Fraction error: " << *this;
        throw runtime_error(str.str());
    }
}


pair<Fraction, Fraction> CommonDenominator
        (const Fraction& a, const Fraction& b)
{
    auto commom_lcm = lcm(a.denominator_, b.denominator_);

    Fraction c(commom_lcm / a.denominator_ * a.numerator_, commom_lcm);
    Fraction d(commom_lcm / b.denominator_ * b.numerator_, commom_lcm);

    return make_pair(c, d);
}