#include<iostream>
#include<random>
#include<chrono>

using namespace std;

int N;
void matrix_mul(float *A, float *B, float *C){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            float sum = 0;
            for(int k = 0; k < N; k++){
                sum += A[i*N + k] * B[k*N + j];
            }
            C[i*N + j] = sum;
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
        matrix_mul(A, B, C);
    }
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;

    cout << "CPU base MM Time: " << diff.count()*1000 << " ms"<< endl;
    
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}