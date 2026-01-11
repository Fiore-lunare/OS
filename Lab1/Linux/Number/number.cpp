#include "number.h"

#include <iostream>

const Number pi(3.14);
const Number m_pi(-3.14);

Number Number::operator+ (const Number& b) const{
    return this->_number + b._number;
}

Number Number::operator- (const Number& b) const{
    return this->_number - b._number;
}

Number Number::operator* (const Number & b) const{
    return this->_number * b._number;
}

Number Number::operator/ (const Number& b) const { // try-catch
    if (b._number == 0) throw "Division by zero"; //check devision
    return this->_number / b._number;
}

Number& Number::operator+=(const Number& b) {
    this->_number += b._number;
    return *this;
}

Number& Number::operator-=(const Number& b) {
    this->_number -= b._number;
    return *this;
}

Number make_number(double number){
    return Number(number);
}

Number Number::square_root() const {
    return Number(sqrt(this->_number));
}

Number Number::arcTan(const Number& y) const {
    const double pi = 3.14159265;
    if (this->_number == 0) {
        if (y._number > 0) return Number(pi / 2);
        else if (y._number < 0) return Number(-pi / 2);
        else return Number(0);
    }
    else if (this->_number > 0) {
        return Number(atan(y._number / this->_number));
    }
    else {
        if (y._number == 0) return Number(pi);
        else if (y._number > 0) return Number(atan(y._number / this->_number) + pi);
        else return Number(atan(y._number / this->_number) - pi);
    }
}