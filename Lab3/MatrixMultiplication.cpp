#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>

#include "MatrixMultiplication.h"


void printIndicies(Indices ind) {
    std::cout<<"lx1: "<<ind.lx1 <<"; ly1: "<<ind.ly1 << "; lx2: "<<ind.lx2 << "; ly2: "<<ind.ly2<<std::endl;
}


Matrix MatrixMultiplication::ordinary_multiplication() {
    int row = matrix1_.size();
    int column = matrix2_[0].size();
    int inner = matrix1_[0].size();

    Matrix result(row, Vector(column));

    for (int i = 0; i < row; i++) {
        for (int cur = 0; cur < column; cur++) {
            for (int j = 0; j < inner; j++) {
                result[i][cur] += (matrix1_[i][j] * matrix2_[j][cur]);
            }
        }
    }
    return result;
}

MatrixMultiplication::MatrixMultiplication(Matrix& matrix1, Matrix& matrix2) {
    matrix1_ = matrix1;
    matrix2_ = matrix2;
    N_ = matrix1.size();
    bc_ = std::make_shared<BufferChannel<Indices>>();
}

void MatrixMultiplication::printMatrix(const Matrix& matrix) {
for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
        std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
}
std::cout << std::endl;
}

void MatrixMultiplication::matrix_block_addition(Matrix& matrix1, const Matrix& matrix2, const Indices& ind) {
    for (int i = 0; i < matrix2.size(); i++) {
        for (int j = 0; j < matrix2[0].size(); j++) {
            matrix1[i + ind.ly1][j + ind.lx1] += matrix2[i][j];
        }
    }
}

void MatrixMultiplication::matrix_block_multiplication(const Indices ind, Matrix& result_main, int k) {
    int ind_end =(k*(N_ / k)), edge_case = N_ - ind_end;

    int row = (ind.ly1 == ind_end )? edge_case: k;
    int column = (ind.lx2 == ind_end)? edge_case: k;
    int inner = (ind.ly2 == ind_end)? edge_case: k;

    Matrix result(row, Vector(column, 0));

    for (int i = 0; i < row; i++) {
        for(int cur= 0; cur < column; cur++) {
            for (int j = 0; j < inner; j++) {
                result[i][cur] += (matrix1_[i + ind.ly1][j + ind.lx1] * matrix2_[j + ind.ly2][cur + ind.lx2]);
            }
        }
    }
    int cur = ind.lx2, i = ind.ly1;

    Indices cur_sum(cur, i);


    int tmp = (N_/ k + ((N_%k) != 0));
    std::lock_guard<std::mutex> lock(*m_[(i/k) * tmp + cur/k]);
    matrix_block_addition(result_main, result, cur_sum);
}

Matrix MatrixMultiplication::matrix_multiplication(int k, int& count) {
    Matrix result(N_, Vector(N_, 0));
    createWorkers(result, k);
    createMutex(result, k);

    for (int i =0; i<N_; i+=k) {
        for (int cur =0; cur<N_; cur+=k) {
            //m_.push_back(std::make_shared<std::mutex>());
            for (int j =0; j<N_; j+=k) {
                Indices ind{j, i,cur, j};
                bc_->Send(ind);
            }
        }
    }
    count = t_.size();
    this->update();
    return result;
}

void MatrixMultiplication:: processor(Matrix& result, int k) {
    std::pair<Indices, bool> res = bc_->Recv();
    while (res.second == true) {
        matrix_block_multiplication(res.first, result, k);
        res = bc_->Recv();
    }
}

void MatrixMultiplication::createWorkers(Matrix& result, int k) {
    int count = bc_->Capacity();

    for (int i=0; i<count; i++) {
        t_.push_back(std::make_shared<std::thread>(&MatrixMultiplication::processor, this, std::ref(result), k));
    }
}

void MatrixMultiplication::createMutex(Matrix& result, int k) {
    for (int i =0; i<N_; i+=k) {
        for (int cur =0; cur<N_; cur+=k) {
            m_.push_back(std::make_shared<std::mutex>());
        }
    }
}

void MatrixMultiplication::update() {
    bc_->Close();
    for (int i = 0; i < t_.size(); i++) {
        t_[i]->join();
    }
    if (!m_.empty()) m_.clear();
    if (!t_.empty()) t_.clear();

    bc_ = std::make_shared<BufferChannel<Indices>>();
}

void MatrixMultiplication::main_function(int k) {

    for (int i = k; i <= N_; i++) {
        std::cout<<"======================== "<<i << ": ======================"<<std::endl;
        int count = 0;
        auto t_start = std::chrono::high_resolution_clock::now();
        Matrix result = matrix_multiplication(i, count);

        auto t_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = t_end - t_start;
        printMatrix(result);
        std::cout<<"block size: "<<i<<std::endl;
        std::cout<<"number of threads: "<<count<<std::endl;
        std::cout<<"time: "<<ms_double.count()<<" ms"<<std::endl;
    }

    std::cout<<"======================== ordinary_multiplication: ======================"<<std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();
    Matrix result = ordinary_multiplication();
    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t_end - t_start;
    printMatrix(result);
    std::cout<<"time: "<<ms_double.count()<<" ms"<<std::endl;
}



