//GPU版本的矩阵相乘
#include <iostream>
#include <random>
#include <cuda_runtime.h>

using namespace std;

__global__ void gemm_baseline(float *A, float *B, float *C,int n){
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        float sum = 0.0f;
        for (int k = 0; k < n; ++k) {
            sum += A[row * n + k] * B[k * n + col];
        }
        C[row * n + col] = sum;
    }

}

void matrixMul(float *A, float *B, float *C, int N, int block_size){
    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, N*N*sizeof(float));
    cudaMalloc(&d_B, N*N*sizeof(float));
    cudaMalloc(&d_C, N*N*sizeof(float));

    cudaMemcpy(d_A, A, N*N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N*N*sizeof(float), cudaMemcpyHostToDevice);

    cudaEvent_t start, end;
    cudaEventCreate(&start);
    cudaEventCreate(&end);

    dim3 block(block_size, block_size);
    dim3 grid((N + block.x - 1) / block.x, (N + block.y - 1) / block.y);

    cudaEventRecord(start, 0);
    gemm_baseline<<<grid, block>>>(d_A, d_B, d_C, N);
    cudaEventRecord(end, 0);

    cudaMemcpy(C, d_C, N*N*sizeof(float), cudaMemcpyDeviceToHost);

    cudaEventSynchronize(end);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, end);

    cout << "MM GPU Time: " << milliseconds << "ms" << endl;

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}



void gemm_verify(float *A, float *B, float *C, int N){
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

int main(int argc, char* argv[]){
  int seed, n, N, block_size;
    if(argc == 2){
        n = atoi(argv[1]);
        seed = time(NULL);
        block_size = 16;
    }
    else if(argc >= 3){
        n = atoi(argv[1]);
        seed = atoi(argv[2]);
        block_size = 16;
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


    matrixMul(A, B, C, N, block_size);

    gemm_verify(A, B, C, N);

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
