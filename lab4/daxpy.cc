#include <cstdio>
#include <random>

#include <gem5/m5ops.h>

void daxpy(double *X, double *Y, double alpha, const int N)
{
    for (int i = 0; i < N; i++)
    {
        Y[i] = alpha * X[i] + Y[i];
    }
}

void daxsbxpxy(double *X, double *Y, double alpha, double beta, const int N)
{
    for (int i = 0; i < N; i++)
    {
        Y[i] = alpha * X[i] * X[i] + beta * X[i] + X[i] * Y[i];
    }
}

void stencil(double *Y, double alpha, const int N)
{
    for (int i = 1; i < N-1; i++)
    {
        Y[i] = alpha * Y[i-1] + Y[i] + alpha * Y[i+1];
    }
}


void daxpy_unroll(double *X, double *Y, double alpha, const int N)
{
    // for (int i = 0; i < N; i++)
    // {
    //     Y[i] = alpha * X[i] + Y[i];
    // }
    int temp = N/4;
    for(int i=0; i<temp*4; i+=4){
        Y[i] = alpha * X[i] + Y[i];
        Y[i+1] = alpha * X[i+1] + Y[i+1];
        Y[i+2] = alpha * X[i+2] + Y[i+2];
        Y[i+3] = alpha * X[i+3] + Y[i+3];
    }

    for(int i=temp*4; i<N; i++){
        Y[i] = alpha * X[i] + Y[i];
    }

}

void daxpy_unroll_x(double *X, double *Y, double alpha, const int N)
{
    // for (int i = 0; i < N; i++)
    // {
    //     Y[i] = alpha * X[i] + Y[i];
    // }
    int m = 16;
    int temp = N/m;
    for(int i=0; i<temp*m; i+=m){
        Y[i] = alpha * X[i] + Y[i];
        Y[i+1] = alpha * X[i+1] + Y[i+1];
        Y[i+2] = alpha * X[i+2] + Y[i+2];
        Y[i+3] = alpha * X[i+3] + Y[i+3];
        Y[i+4] = alpha * X[i+4] + Y[i+4];
        Y[i+5] = alpha * X[i+5] + Y[i+5];
        Y[i+6] = alpha * X[i+6] + Y[i+6];
        Y[i+7] = alpha * X[i+7] + Y[i+7];
        Y[i+8] = alpha * X[i+8] + Y[i+8];
        Y[i+9] = alpha * X[i+9] + Y[i+9];
        Y[i+10] = alpha * X[i+10] + Y[i+10];
        Y[i+11] = alpha * X[i+11] + Y[i+11];
        Y[i+12] = alpha * X[i+12] + Y[i+12];
        Y[i+13] = alpha * X[i+13] + Y[i+13];
        Y[i+14] = alpha * X[i+14] + Y[i+14];
        Y[i+15] = alpha * X[i+15] + Y[i+15];
        // Y[i+16] = alpha * X[i+16] + Y[i+16];
        // Y[i+17] = alpha * X[i+17] + Y[i+17];
        // Y[i+18] = alpha * X[i+18] + Y[i+18];
        // Y[i+19] = alpha * X[i+19] + Y[i+19];
        // Y[i+20] = alpha * X[i+20] + Y[i+20];
        // Y[i+21] = alpha * X[i+21] + Y[i+21];
        // Y[i+22] = alpha * X[i+22] + Y[i+22];
        // Y[i+23] = alpha * X[i+23] + Y[i+23];
        // Y[i+24] = alpha * X[i+24] + Y[i+24];
        // Y[i+25] = alpha * X[i+25] + Y[i+25];
        // Y[i+26] = alpha * X[i+26] + Y[i+26];
        // Y[i+27] = alpha * X[i+27] + Y[i+27];
        // Y[i+28] = alpha * X[i+28] + Y[i+28];
        // Y[i+29] = alpha * X[i+29] + Y[i+29];
        // Y[i+30] = alpha * X[i+30] + Y[i+30];
        // Y[i+31] = alpha * X[i+31] + Y[i+31];
    }

    for(int i=temp*m; i<N; i++){
        Y[i] = alpha * X[i] + Y[i];
    }

}

void daxsbxpxy_unroll(double *X, double *Y, double alpha, double beta, const int N)
{
    // for (int i = 0; i < N; i++)
    // {
    //     Y[i] = alpha * X[i] * X[i] + beta * X[i] + X[i] * Y[i];
    // }
    int temp = N/4;
    for(int i=0; i<temp*4; i+=4){
        Y[i] = alpha * X[i] * X[i] + beta * X[i] + X[i] * Y[i];
        Y[i+1] = alpha * X[i+1] * X[i+1] + beta * X[i+1] + X[i+1] * Y[i+1];
        Y[i+2] = alpha * X[i+2] * X[i+2] + beta * X[i+2] + X[i+2] * Y[i+2];
        Y[i+3] = alpha * X[i+3] * X[i+3] + beta * X[i+3] + X[i+3] * Y[i+3];
    }

    for(int i=temp*4; i<N; i++){
        Y[i] = alpha * X[i] * X[i] + beta * X[i] + X[i] * Y[i];
    }
}

void stencil_unroll(double *Y, double alpha, const int N)
{
    // for (int i = 1; i < N-1; i++)
    // {
    //     Y[i] = alpha * Y[i-1] + Y[i] + alpha * Y[i+1];
    // }
    int temp = (N-2)/4;
    for(int i=0; i<temp*4; i+=4){
        Y[i+1] = alpha * Y[i] + Y[i+1] + alpha * Y[i+2];
        Y[i+2] = alpha * Y[i+1] + Y[i+2] + alpha * Y[i+3];
        Y[i+3] = alpha * Y[i+2] + Y[i+3] + alpha * Y[i+4];
        Y[i+4] = alpha * Y[i+3] + Y[i+4] + alpha * Y[i+5];
    }

    for(int i=temp*4; i<N-2; i++){
        Y[i+1] = alpha * Y[i] + Y[i+1] + alpha * Y[i+2];
    }
}


void stencil_unroll_x(double *Y, double alpha, const int N)
{
    // for (int i = 1; i < N-1; i++)
    // {
    //     Y[i] = alpha * Y[i-1] + Y[i] + alpha * Y[i+1];
    // }
    int m = 16;
    int temp = (N-2)/m;
    for(int i=0; i<temp*m; i+=m){
        Y[i+1] = alpha * Y[i] + Y[i+1] + alpha * Y[i+2];
        Y[i+2] = alpha * Y[i+1] + Y[i+2] + alpha * Y[i+3];
        Y[i+3] = alpha * Y[i+2] + Y[i+3] + alpha * Y[i+4];
        Y[i+4] = alpha * Y[i+3] + Y[i+4] + alpha * Y[i+5];
        Y[i+5] = alpha * Y[i+4] + Y[i+5] + alpha * Y[i+6];
        Y[i+6] = alpha * Y[i+5] + Y[i+6] + alpha * Y[i+7];
        Y[i+7] = alpha * Y[i+6] + Y[i+7] + alpha * Y[i+8];
        Y[i+8] = alpha * Y[i+7] + Y[i+8] + alpha * Y[i+9];
        Y[i+9] = alpha * Y[i+8] + Y[i+9] + alpha * Y[i+10];
        Y[i+10] = alpha * Y[i+9] + Y[i+10] + alpha * Y[i+11];
        Y[i+11] = alpha * Y[i+10] + Y[i+11] + alpha * Y[i+12];
        Y[i+12] = alpha * Y[i+11] + Y[i+12] + alpha * Y[i+13];
        Y[i+13] = alpha * Y[i+12] + Y[i+13] + alpha * Y[i+14];
        Y[i+14] = alpha * Y[i+13] + Y[i+14] + alpha * Y[i+15];
        Y[i+15] = alpha * Y[i+14] + Y[i+15] + alpha * Y[i+16];
        Y[i+16] = alpha * Y[i+15] + Y[i+16] + alpha * Y[i+17];
        // Y[i+17] = alpha * Y[i+16] + Y[i+17] + alpha * Y[i+18];
        // Y[i+18] = alpha * Y[i+17] + Y[i+18] + alpha * Y[i+19];
        // Y[i+19] = alpha * Y[i+18] + Y[i+19] + alpha * Y[i+20];
        // Y[i+20] = alpha * Y[i+19] + Y[i+20] + alpha * Y[i+21];
        // Y[i+21] = alpha * Y[i+20] + Y[i+21] + alpha * Y[i+22];
        // Y[i+22] = alpha * Y[i+21] + Y[i+22] + alpha * Y[i+23];
        // Y[i+23] = alpha * Y[i+22] + Y[i+23] + alpha * Y[i+24];
        // Y[i+24] = alpha * Y[i+23] + Y[i+24] + alpha * Y[i+25];
        // Y[i+25] = alpha * Y[i+24] + Y[i+25] + alpha * Y[i+26];
        // Y[i+26] = alpha * Y[i+25] + Y[i+26] + alpha * Y[i+27];
        // Y[i+27] = alpha * Y[i+26] + Y[i+27] + alpha * Y[i+28];
        // Y[i+28] = alpha * Y[i+27] + Y[i+28] + alpha * Y[i+29];
        // Y[i+29] = alpha * Y[i+28] + Y[i+29] + alpha * Y[i+30];
        // Y[i+30] = alpha * Y[i+29] + Y[i+30] + alpha * Y[i+31];
        // Y[i+31] = alpha * Y[i+30] + Y[i+31] + alpha * Y[i+32];
        // Y[i+32] = alpha * Y[i+31] + Y[i+32] + alpha * Y[i+33];
    }

    for(int i=temp*m; i<N-2; i++){
        Y[i+1] = alpha * Y[i] + Y[i+1] + alpha * Y[i+2];
    }
}

void diff(double *Y, double *Y_unroll, const int N, int times)
{
    switch(times){
        case 1:
            printf("daxpy:");
            break;
        case 2:
            printf("daxsbxpxy:");
            break;
        case 3:
            printf("stencil:");
            break;
        default: break;
    }
    for (int i = 0; i < N; i++)
    {
        if (Y[i] != Y_unroll[i])
        {
            printf("Error: Y[%d] = %lf, Y_unroll[%d] = %lf\n", i, Y[i], i, Y_unroll[i]);
            return;
        }
    }
    printf("Correct\n");
}


int main(int argc,char *argv[])
{
    const int N = 10000;
    double *X = new double[N], *Y = new double[N], alpha = 0.5, beta = 0.1;
    double *Y_unroll = new double[N];

    //std::random_device rd;
    std::mt19937 gen(0);
    std::uniform_real_distribution<> dis(1, 2);
    for (int i = 0; i < N; ++i)
    {
        X[i] = dis(gen);
        Y[i] = dis(gen);
        Y_unroll[i] = Y[i];
    }

    m5_dump_reset_stats(0, 0);
    daxpy(X, Y, alpha, N);                              //2
    m5_dump_reset_stats(0, 0);
    daxpy_unroll_x(X, Y_unroll, alpha, N);                //3
    m5_dump_reset_stats(0, 0);
    diff(Y, Y_unroll, N, 1);
    m5_dump_reset_stats(0, 0);
    daxsbxpxy(X, Y, alpha, beta, N);                    //5
    m5_dump_reset_stats(0, 0);
    daxsbxpxy_unroll(X, Y_unroll, alpha, beta, N);      //6
    m5_dump_reset_stats(0, 0);
    diff(Y, Y_unroll, N, 2);
    m5_dump_reset_stats(0, 0);
    stencil(Y, alpha, N);                               //8
    m5_dump_reset_stats(0, 0);
    stencil_unroll_x(Y_unroll, alpha, N);                 //9
    m5_dump_reset_stats(0, 0);
    diff(Y, Y_unroll, N, 3);

    double sum = 0;
    for (int i = 0; i < N; ++i)
    {
        sum += Y[i];
    }
    printf("%lf\n", sum);
    return 0;
}
