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

void gemm_avx_blocks(float *A, float *B, float *C, int block_size){
    float *B_T = new float[N*N];
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            B_T[i*N + j] = B[j*N + i];
        }
    }

    for(int i = 0; i < N; i += block_size){
        for(int j = 0; j < N; j += block_size){
            for(int k = 0; k < N; k += block_size){
                for(int ii = i; ii < i + block_size; ii++){
                    for(int jj = j; jj < j + block_size; jj++){
                        __m256 c = _mm256_setzero_ps();
                        for(int kk = k; kk < k + block_size; kk += 8){
                            __m256 a = _mm256_loadu_ps(A + ii*N + kk);
                            __m256 b = _mm256_loadu_ps(B_T + jj*N + kk);
                            c = _mm256_fmadd_ps(a, b, c);
                        }
                        C[ii*N + jj] += c[0] + c[1] + c[2] + c[3] + c[4] + c[5] + c[6] + c[7];
                    }
                }
            }
        }
    }

    delete[] B_T;}


int main(int argc, char* argv[]){
    int seed, n, block_size;
    if(argc == 2){
        n = atoi(argv[1]);
        seed = time(NULL);
        block_size = 64;
    }
    else if(argc == 3){
        n = atoi(argv[1]);
        seed = atoi(argv[2]);
        block_size = 64;
    }
    else{
        n = atoi(argv[1]);
        seed = atoi(argv[2]);
        block_size = atoi(argv[3]);
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
    gemm_avx_blocks(A, B, C, block_size);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;

    cout << "CPU base AVX MM BLOCKS UP Time: " << diff.count()*1000 <<" ms" << endl;

    gemm_verify(A, B, C);

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}