#pragma once

class Number {
private:
	double _number;
public:

	Number() {
		_number = 0;
	}

	Number(double number) : _number(number) {}
	~Number() = default;

	double GetNumber() {
		return _number;
	}

	void SetNumber(double number) {
		_number = number;
	}

	Number operator+(const Number&) const;
	Number operator-(const Number&) const;
	Number operator*(const Number&) const;
	Number operator/(const Number&) const;
	Number& operator+= (const Number&);
	Number& operator-= (const Number&);
	Number square_root() const;
	Number arcTan(const Number&) const;

};

Number make_number(double number);