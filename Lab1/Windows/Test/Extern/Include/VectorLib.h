// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the VECTORLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// VECTORLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma  once 
#include "NumberLib.h"

#ifdef VECTORLIB_EXPORTS
#define VECTORLIB_API __declspec(dllexport)
#else
#define VECTORLIB_API __declspec(dllimport)
#endif

class VECTORLIB_API Vector
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

extern "C" VECTORLIB_API
Vector* CreateVectorWithParameters(Number*, Number*);

extern "C" VECTORLIB_API
Vector* CreateVector();

extern "C" VECTORLIB_API
void DeleteVector(Vector*);

extern "C" VECTORLIB_API
Number GetR(Vector*);

