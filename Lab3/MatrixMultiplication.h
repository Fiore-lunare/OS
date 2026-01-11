#ifndef BUFFERCHANNEL_MATRIXMULTIPLICATION_H
#define BUFFERCHANNEL_MATRIXMULTIPLICATION_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>

#include "BufferChannel.h"

using Matrix =  std::vector<std::vector<int>>;
using Vector = std::vector<int>;

struct Indices {
    int lx1;
    int ly1;
    int lx2 = -1;
    int ly2 = -1;
};

void printIndicies(Indices ind);

class MatrixMultiplication {
    Matrix matrix1_;
    Matrix matrix2_;
    std::shared_ptr<BufferChannel<Indices>> bc_;
    std::vector<std::shared_ptr<std::mutex>> m_;
    std::vector<std::shared_ptr<std::thread>> t_;
    int N_;
    std::mutex mat;

public:
    Matrix ordinary_multiplication();
    MatrixMultiplication(Matrix& matrix1, Matrix& matrix2);
    void printMatrix(const Matrix& matrix);


private:
    void matrix_block_addition(Matrix& matrix1, const Matrix& matrix2, const Indices& ind);
    void matrix_block_multiplication(const Indices ind, Matrix& result_main, int k);

    Matrix matrix_multiplication(int k, int& count);

    void processor(Matrix& result, int k);

    void createWorkers(Matrix& result, int k);

    void update();
    void createMutex(Matrix& result, int k);
public:
    void main_function(int k);

};






#endif //BUFFERCHANNEL_MATRIXMULTIPLICATION_H