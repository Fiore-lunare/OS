#ifndef VECTOR_LIBRARY_H
#define VECTOR_LIBRARY_H

#include "Vector_Export.h"
#include "number.h"

class VECTOR_API Vector
{
private:
    Number x_;
    Number y_;
public:
    Vector();
    Vector(Number, Number);

    Vector operator+(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector operator-(const Vector& v) const;
    Vector& operator-=(const Vector& v);
    Number r() const;
    Number a() const;
};

extern "C"{
    VECTOR_API Vector* CreateVector();
    VECTOR_API void DeleteVector(Vector*);
    VECTOR_API Number getRadius(Vector* v);
    VECTOR_API Vector* CreateVector2(Number, Number);
}

#endif // VECTOR_LIBRARY_H