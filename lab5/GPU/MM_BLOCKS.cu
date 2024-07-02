//GPU版本的矩阵相乘
#include <iostream>
#include <random>
#include <cuda_runtime.h>

using namespace std;
#define BLOCK_SIZE 64

__global__ void gemm_blocks(float *A, float *B, float *C,int N){
    if ((blockIdx.y * blockDim.y + threadIdx.y) * N + blockIdx.x * blockDim.x + threadIdx.x >= N * N) {
        return;
    }

    const int begin_A = blockIdx.y * blockDim.y * N;
    const int end_A = begin_A + N - 1;
    const int step_A = blockDim.x;

    const int begin_B = blockIdx.x * blockDim.x;
    const int step_B = blockDim.y * N;

    float result_temp = 0.0f;
    int i_B = begin_B;
    for (int i_A=begin_A; i_A < end_A; i_A += step_A, i_B += step_B)  {
        // shared memory
        __shared__ float ASum[BLOCK_SIZE][BLOCK_SIZE];
        __shared__ float BSum[BLOCK_SIZE][BLOCK_SIZE];

        // copy data to shared memory
        ASum[threadIdx.y][threadIdx.x] = A[i_A + threadIdx.y * N + threadIdx.x];
        BSum[threadIdx.y][threadIdx.x] = B[i_B + threadIdx.y * N + threadIdx.x];

        __syncthreads();

        for (int i = 0; i < BLOCK_SIZE; i++) {
            result_temp += ASum[threadIdx.y][i] * BSum[i][threadIdx.x];
        }

        __syncthreads();
    }

    C[blockIdx.y * blockDim.y * N + begin_B + threadIdx.y * N + threadIdx.x] = result_temp;
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

    // dim3 block(16, 16);
    // dim3 grid((N + block.x - 1) / block.x, (N + block.y - 1) / block.y);
    dim3 block(block_size, block_size);
    dim3 grid((N + block.x - 1) / block.x, (N + block.y - 1) / block.y);

    cudaEventRecord(start,   0);
    gemm_blocks<<<grid, block>>>(d_A, d_B, d_C, N);
    cudaEventRecord(end, 0);

    cudaMemcpy(C, d_C, N*N*sizeof(float), cudaMemcpyDeviceToHost);

    cudaEventSynchronize(end);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, end);

    cout << "MM_BLOCKS GPU Time: " << milliseconds << "ms" << endl;

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
    else if(argc == 3){
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

