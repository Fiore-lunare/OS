#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>



using Matrix =  std::vector<std::vector<int>>;
using Vector = std::vector<int>;

Matrix ordinary_multiplication(const Matrix& matrix1, const Matrix& matrix2) {
    int row = matrix1.size();
    int column = matrix2[0].size();
    int inner = matrix1[0].size();

    Matrix result(row, Vector(column));

    for (int i = 0; i < row; i++) {
        for (int cur = 0; cur < column; cur++) {
            for (int j = 0; j < inner; j++) {
                result[i][cur] += (matrix1[i][j] * matrix2[j][cur]);
            }
        }
    }
    return result;
}

struct Indices {
    int lx1;
    int ly1;
    int lx2 = -1;
    int ly2 = -1;

    int row = -1;
    int column = -1;
    int inner = -1;
};

void printMatrix(const Matrix& matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void matrix_block_addition(Matrix& matrix1, const Matrix& matrix2, const Indices& ind) {
    for (int i = 0; i < matrix2.size(); i++) {
        for (int j = 0; j < matrix2[0].size(); j++) {
            matrix1[i + ind.ly1][j + ind.lx1] += matrix2[i][j];
        }
    }
}

void matrix_block_multiplication(const Matrix& matrix1, const Matrix& matrix2, const Indices ind, Matrix& result_main, const Indices cur_sum, std::shared_ptr<std::mutex> mutex) {
    Matrix result(ind.row, Vector(ind.column, 0));
    for (int i = 0; i < ind.row; i++) {
        for(int cur= 0; cur < ind.column; cur++) {
            for (int j = 0; j < ind.inner; j++) {
                result[i][cur] += (matrix1[i + ind.ly1][j + ind.lx1] * matrix2[j + ind.ly2][cur + ind.lx2]);
            }
        }
    }
    std::lock_guard<std::mutex> lock(*mutex);
    matrix_block_addition(result_main, result, cur_sum);
}

Matrix matrix_multiplication(const Matrix& matrix1, const Matrix& matrix2, int N, int k, int& count) {
    Matrix result(N, Vector(N, 0));
    Matrix temp;

    Indices ind;

    std::vector<std::shared_ptr<std::thread>> v;
    std::vector<std::shared_ptr<std::mutex>> m;
    int ind_end =(k*(N / k)), edge_case = N - ind_end;
    for (int i =0; i<N; i+=k) {
        for (int cur =0; cur<N; cur+=k) {
            Indices cur_sum(cur, i);
            m.push_back(std::make_shared<std::mutex>()); //when will these objects dye?
            for (int j =0; j<N; j+=k) {
                int row = (i == ind_end )? edge_case: k;
                int column = (cur == ind_end)? edge_case: k;
                int inner = (j == ind_end)? edge_case: k;
                Indices ind(j, i,cur, j, row, column, inner);
                v.push_back(std::make_shared<std::thread>(matrix_block_multiplication, matrix1, matrix2, ind, std::ref(result), cur_sum, m.back()));
            }
        }
    }
    for (int i = 0; i < v.size(); i++) {
        v[i]->join();
    }
    count = v.size();
    return result;
}

void main_function(const Matrix& matrix1, const Matrix& matrix2, int k, int N) {

    for (int i = k; i <= N; i++) {
        std::cout<<"======================== "<<i << ": ======================"<<std::endl;
        int count = 0;
        auto t_start = std::chrono::high_resolution_clock::now();
        Matrix result = matrix_multiplication(matrix1, matrix2, N, i, count);
        auto t_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = t_end - t_start;
        printMatrix(result);
        std::cout<<"block size: "<<i<<std::endl;
        std::cout<<"number of threads: "<<count<<std::endl;
        std::cout<<"time: "<<ms_double.count()<<" ms"<<std::endl;
    }

    std::cout<<"======================== ordinary_multiplication: ======================"<<std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();
    Matrix result = ordinary_multiplication(matrix1, matrix2);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t_end - t_start;
    printMatrix(result);
    std::cout<<"time: "<<ms_double.count()<<" ms"<<std::endl;
}

void generate_Main_Function(int k, int size) {
    if (size == 0 || k == 0) {
        throw std::invalid_argument("invalid argument");
    }
    Matrix Cc (size, Vector(size, 0));
    Matrix Dd(size, Vector(size, 0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Cc[i][j] = std::rand() %101 - 50;
            Dd[i][j] = std::rand() %101 - 50;
        }
    }
    main_function(Cc, Dd, k, size);
}

int main() {
    srand(time(NULL));
    generate_Main_Function(10, 50);
}