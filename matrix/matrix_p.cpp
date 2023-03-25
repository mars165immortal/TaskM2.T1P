#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

const int N = 10; // Size of matrices
const int THREADS = 6; // Number of threads to use

struct MatrixData {
    int start;
    int end;
    int** A;
    int** B;
    int** R;
};

void initialize_matrix(int **matrix) {
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            matrix[i][j] = rand() % 100; // Initialize with random values
        }
    }
}

void* compute_block(void* arg) {
    MatrixData* data = (MatrixData*)arg;
    for(int i=data->start; i<data->end; i++) {
        for(int j=0; j<N; j++) {
            data->R[i][j] = 0;
            for(int k=0; k<N; k++) {
                data->R[i][j] += data->A[i][k] * data->B[k][j]; // Compute R[i][j]
            }
        }
    }
    pthread_exit(NULL);
}

void matrix_multiplication(int **A, int **B, int **R) {
    pthread_t threads[THREADS];
    MatrixData data[THREADS];
    int block_size = N / THREADS;
    int remainder = N % THREADS;

    for(int i=0; i<THREADS; i++) {
        data[i].A = A;
        data[i].B = B;
        data[i].R = R;
        data[i].start = i * block_size;
        data[i].end = data[i].start + block_size;
        if(i == THREADS - 1) {
            data[i].end += remainder;
        }
        pthread_create(&threads[i], NULL, compute_block, (void*)&data[i]);
    }

    for(int i=0; i<THREADS; i++) {
        pthread_join(threads[i], NULL);
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
