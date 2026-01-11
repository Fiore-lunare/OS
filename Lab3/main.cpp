#include <iostream>
#include "MatrixMultiplication.h"

using Matrix =  std::vector<std::vector<int>>;
using Vector = std::vector<int>;


void generate_Main_Function(int k, int size) {
    if (size <= 0 || k <= 0) {
        throw std::invalid_argument("Invalid argument");
    }
    Matrix Cc (size, Vector(size, 0));
    Matrix Dd(size, Vector(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Cc[i][j] = std::rand() %101 - 50;
            Dd[i][j] = std::rand() %101 - 50;
        }
    }
    MatrixMultiplication m (Cc, Dd);
    std::cout<<"First matrix:\n";
    m.printMatrix(Cc);
    std::cout<<"Second matrix:\n";
    m.printMatrix(Dd);
    m.main_function(k);
}

int main() {
    srand(time(NULL));

    generate_Main_Function(3,7);


}

