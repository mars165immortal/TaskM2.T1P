#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

const int N = 10; // Size of matrices

void initialize_matrix(int **matrix) {
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            matrix[i][j] = rand() % 100; // Initialize with random values
        }
    }
}

void matrix_multiplication(int **A, int **B, int **R) {
    #pragma omp parallel for
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            R[i][j] = 0;
            for(int k=0; k<N; k++) {
                R[i][j] += A[i][k] * B[k][j]; // Compute R[i][j]
            }
        }
    }
}

void print_matrix(int **A, int **B, int **R) {
    cout << "Matrix A:" << endl;
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Matrix B:" << endl;
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            cout << B[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Matrix R:" << endl;
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            cout << R[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    srand(time(NULL)); // Seed random number generator with current time

    int **A, **B, **R;

    // Allocate memory for matrices
    A = new int*[N];
    B = new int*[N];
    R = new int*[N];
    for(int i=0; i<N; i++) {
        A[i] = new int[N];
        B[i] = new int[N];
        R[i] = new int[N];
    }

    // Initialize matrices A and B with random values
    initialize_matrix(A);
    initialize_matrix(B);

    // Measure the CPU time used by the program
    clock_t start_time = clock();

    // Compute matrix multiplication of A and B to get matrix R
    matrix_multiplication(A, B, R);

    // Print matrices A, B, and R to console
    print_matrix(A, B, R);

    // Calculate and print the execution time of the matrix multiplication
    clock_t end_time = clock();
    double execution_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "Execution time: " << execution_time << " seconds" << endl;

    // Free memory used by matrices
    for(int i=0; i<N; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] R[i];
    }
    delete[] A;
    delete[] B;
    delete[] R;

    return 0;
}
