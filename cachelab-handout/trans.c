#include <stdio.h>

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Dante Niewenhuis = 11058595
 * Lodewijk van Keizerswaard = 11054115
 * 
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, iterI, iterJ, block_i, block_j, temp, temp_made = 0;

    if (M == 32) {
        block_i = 8;
        block_j = 8;

        for (iterI = 0; iterI < 32; iterI += block_i) {
            for (iterJ = 0; iterJ < 32; iterJ += block_j) {
                for (i = iterI; i < (iterI + block_i); i++) {
                    for (j = iterJ; j < (iterJ + block_j); j++) {
                        if (i == j) {
                            temp = A[i][j];
                            temp_made = 1;
                        }
                        else {
                            B[j][i] = A[i][j];
                        }
                    }
                    if (temp_made) {
                        temp_made = 0;
                        B[i][i] = temp;
                    }
                }
            }
        }
    }

    if (M == 61) {
        block_i = 4;
        block_j = 16;

        for (iterJ = 0; iterJ < 60; iterJ += block_j) {
            for (iterI = 0; iterI < 68; iterI += block_i) {
                for (j = iterJ; j < (iterJ + block_j); j++) {
                    for (i = iterI; (i < (iterI + block_i)) && (i < 67); i++) {
                        if (i == j) {
                            temp = A[i][j];
                            temp_made = 1;
                        }
                        else {
                            B[j][i] = A[i][j];
                        }
                    }
                    if (temp_made) {
                        temp_made = 0;
                        B[j][j] = temp;
                    }
                }
            }
        }

        for (i = 66; i >= 0; i--) {
            B[j][i] = A[i][j];
        }
    }

    if (M == 64) {
        block_i = 64;
        block_j = 4;


        for (iterJ = 0; iterJ < 64; iterJ += block_j) {
            if (iterJ % (block_j*2) == 0) {
                for (iterI = 0; iterI < 64; iterI += block_i) {
                    for (i = iterI; i < (iterI + block_i); i++) {
                        for (j = iterJ; j < (iterJ + block_j); j++) {
                            if (i == j) {
                                temp = A[i][j];
                                temp_made = 1;
                            }
                            else {
                                B[j][i] = A[i][j];
                            }
                        }
                        if (temp_made) {
                            temp_made = 0;
                            B[i][i] = temp;
                        }
                    }
                }
            }
            else {
                for (iterI = 64-block_i; iterI >= 0; iterI -= block_i) {
                    for (i = (iterI + block_i - 1); i >= iterI ; i--) {
                        for (j = iterJ; j < (iterJ + block_j); j++) {
                            if (i == j) {
                                temp = A[i][j];
                                temp_made = 1;
                            }
                            else {
                                B[j][i] = A[i][j];
                            }
                        }
                        if (temp_made) {
                            temp_made = 0;
                            B[i][i] = temp;
                        }
                    }
                }
            }
        }   
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

