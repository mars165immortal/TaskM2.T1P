#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <omp.h>

using namespace std;

const int N = 100; // Size of matrices
const int NUM_THREADS = 4; // Number of threads for pthreads implementation

int **A, **B, **R;

void initialize_matrix(int **matrix) {
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            matrix[i][j] = rand() % 100; // Initialize with random values
        }
    }
}

void matrix_multiplication_sequential() {
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            R[i][j] = 0;
            for(int k=0; k<N; k++) {
                R[i][j] += A[i][k] * B[k][j]; // Compute R[i][j]
            }
        }
    }
}

void *matrix_multiplication_pthread(void *thread_id) {
    long tid = (long)thread_id;
    int chunk_size = N / NUM_THREADS;
    int start = tid * chunk_size;
    int end = (tid == NUM_THREADS-1) ? N : (tid + 1) * chunk_size;

    for(int i=start; i<end; i++) {
        for(int j=0; j<N; j++) {
            R[i][j] = 0;
            for(int k=0; k<N; k++) {
                R[i][j] += A[i][k] * B[k][j]; // Compute R[i][j]
            }
        }
    }

    pthread_exit(NULL);
}

void matrix_multiplication_omp() {
    #pragma omp parallel for schedule(static)
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            R[i][j] = 0;
            for(int k=0; k<N; k++) {
                R[i][j] += A[i][k] * B[k][j]; // Compute R[i][j]
            }
        }
    }
}

void print_matrix() {
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
    // Allocate memory for matrices
    A = new int*[N];
    B = new int*[N];
    R = new int*[N];
    for(int i=0; i<N; i++) {
        A[i] = new int[N];
        B[i] = new int[N];
        R[i] = new int[N];
    }

    // Initialize matrices with random values
    srand(time(NULL));
    initialize_matrix(A);
    initialize_matrix(B);

    // Sequential implementation
    clock_t start_time = clock();
    matrix_multiplication_sequential();
    clock_t end_time = clock();
    cout << "Sequential time: " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << endl;

    // Pthreads implementation
    pthread_t threads[NUM_THREADS];
    start_time = clock();
    for(long i=0; i<NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, matrix_multiplication_pthread, (void *)i);
    }
    for(int i=0; i<NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    end_time = clock();
    cout << "Pthreads time: " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << endl;

    // OpenMP implementation
    start_time = clock();
    matrix_multiplication_omp();
    end_time = clock();
    cout << "OpenMP time: " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds" << endl;

    // Print the matrices
    print_matrix();

    // Free memory allocated for matrices
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
