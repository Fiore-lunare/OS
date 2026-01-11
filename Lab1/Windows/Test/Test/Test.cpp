#include <iostream>
#include <windows.h>
#include "NumberLib.h"

typedef void* (*CreateVector)();
typedef void (*DeleteVector)(void*);
typedef void* (*CreateVectorWithParam) (Number*, Number*);
typedef Number(*GetR)(void*);



void test1() {
	//this works without #include "VectorLib.h"
    HMODULE DLL = LoadLibraryA("VectorLib.dll"); // HMODULE DLL = LoadLibrary(L"VectorLib.dll");

    if (!DLL) {
        std::cerr << "Failed to load Vector.dll" << std::endl;
        return;
    }

    CreateVector CreateVectorf = (CreateVector)GetProcAddress(DLL, "CreateVector");
    CreateVectorWithParam CreateVectorWithParamf = (CreateVectorWithParam)GetProcAddress(DLL, "CreateVectorWithParameters");
    DeleteVector DeleteVectorf = (DeleteVector)GetProcAddress(DLL, "DeleteVector");
    GetR GetRf = (GetR)GetProcAddress(DLL, "GetR");

    Number* n1 = new Number(3.0);
    Number* n2 = new Number(4.1);

    void* v = CreateVectorf();
    void* v1 = CreateVectorWithParamf(n1,n2);

    std::cout << GetRf(v1).GetNumber() << std::endl;
    std::cout << GetRf(v).GetNumber() << std::endl;

    DeleteVectorf(v);
    DeleteVectorf(v1);

    delete n1;
    delete n2;
}


//to test this method comment all typedef and test1
/*#include "VectorLib.h"

void test2() {
    Number n1(3);
    Number n2(4);

    Number nSum = n1 + n2;
    Number nMul = n1 * n2;
	Number nSub = n1 - n2;
	Number nDiv = n1 / n2;

    std::cout << "nSum: " << nSum.GetNumber() << std::endl;
    std::cout << "nMul: " << nMul.GetNumber() << std::endl;
	std::cout << "nSub: " << nSub.GetNumber() << std::endl;
	std::cout << "nDiv: " << nDiv.GetNumber() << std::endl;
	std::cout << "n2 root: " << n2.square_root().GetNumber() << std::endl;
    
    
	Vector vSum = Vector(n1, n2) + Vector(n2, n1);
	Vector vSub = Vector(n1, n2) - Vector(n2, n1);
    std::cout << "vSum r: " << vSum.r().GetNumber() << std::endl;
    std::cout << "vSum a: " << vSum.a().GetNumber() << std::endl;

	
}*/



int main()
{
    test1();
}
