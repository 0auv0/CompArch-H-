#include<iostream>
#include<immintrin.h>
#include<random>
#include<cmath>
#include<chrono>

using namespace std;

int N;

void gemm_verify(float *A, float *B, float *C){
    float *temp = new float[N*N];
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            float sum = 0;
            for(int k = 0; k < N; k++){
                sum += A[i*N + k] * B[k*N + j];
            }
            temp[i*N + j] = sum;
        }
    }

    for(int i = 0; i < N*N; i++){
        if(fabs(temp[i] - C[i]) > 1e-3){
            cout << "Error at " << i << endl;
            cout << "temp: " << temp[i] << " C: " << C[i] << endl;
            cout << "Error: " << fabs(temp[i] - C[i]) << endl;
            delete[] temp;
            return;
        }
    }

    cout << "Correct" << endl;
    delete[] temp;
    return;
}


void gemm_avx(float *A, float *B, float *C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j+=8) {
            __m256 c = _mm256_setzero_ps(); 
            for (int k = 0; k < N; k++) {
                __m256 a = _mm256_broadcast_ss(&A[i*N+k]); 
                __m256 b = _mm256_loadu_ps(&B[k*N+j]); 
                c = _mm256_fmadd_ps(a, b, c); 
            }
            _mm256_storeu_ps(&C[i*N+j], c);
        }
    }
}



int main(int argc, char* argv[]){
    int seed, n;
    if(argc == 2){
        n = atoi(argv[1]);
        seed = time(NULL);
    }
    else if(argc >= 3){
        n = atoi(argv[1]);
        seed = atoi(argv[2]);
    }

    N = 1 << n;
    float *A = new float[N*N];
    float *B = new float[N*N];
    float *C = new float[N*N];

    srand(seed);
    for(int i = 0; i < N*N; i++){
        A[i] = (float)rand() / RAND_MAX;
        B[i] = (float)rand() / RAND_MAX;
    }

    auto start = chrono::high_resolution_clock::now();
    for(int times = 0; times < 1; times++){
        gemm_avx(A, B, C);
    }
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;

    cout << "CPU base AVX MM Time: " << diff.count() * 1000<<" ms" << endl;

    gemm_verify(A, B, C);

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}