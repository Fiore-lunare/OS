#include <iostream>
#include "number.h"
#include "vector.h"
#include "VectorConstants.h"

int main() {
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

    const Vector vC = zero;


    Vector vSum = Vector(n1, n2) + Vector(n2, n1);
    Vector vSub = Vector(n1, n2) - Vector(n2, n1);
    std::cout << "vSum r: " << vSum.r().GetNumber() << std::endl;
    std::cout << "vSum a: " << vSum.a().GetNumber() << std::endl;

}