#include "cuda_runtime.h"
#include "device_launch_parameters.h"
// added math library
#include<math.h>
#include <stdio.h>
// added one set matrix dimension
#define SIZE 3

cudaError_t addWithCuda(int c[][SIZE], const int a[][SIZE], const int b[][SIZE], unsigned int size, dim3 threads);

// changed the parameters from 1D vectors to 2D matrices with one dimension set
__global__ void addKernel(int c[][SIZE], const int a[][SIZE], const int b[][SIZE])
{
    // Modified index i for several blocks
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    // added index for second matrix dimension
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    // changed the indexing of the arrays to accessing matrix elements
    c[i][j] = a[i][j] + b[i][j];
}

int main()
{
    const int matrixSizeX = SIZE;
    const int matrixSizeY = SIZE;
    // changed into 2D arrays
    const int a[matrixSizeX][SIZE] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9 } };
    const int b[matrixSizeX][SIZE] = { {10, 20, 30}, {40, 50, 60}, {70, 80, 90 } };
    int c[matrixSizeX][SIZE] = { 0 };

    // Added variable for storing threads number
    dim3 threads(2, 2);

    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(c, a, b, (matrixSizeX * matrixSizeY), threads);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    // Modified result printing
    printf("{{1, 2, 3}, {4, 5, 6}, {7, 8, 9 }} + {{10, 20, 30}, {40, 50, 60}, {70, 80, 90}} = {{%d,%d,%d},{%d,%d,%d},{%d,%d,%d}}\n",
       c[0][0], c[0][1], c[0][2], c[1][0], c[1][1], c[1][2], c[2][0], c[2][1], c[2][2] );

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

// Helper function for using CUDA to add vectors in parallel.
// added number of threads (block size) as a parameter
// added total matrix size as a parameter for proper memory allocation
// changed 1D arrays for the 2D matrices with one dimension set - C did not allow dynamic arrays
cudaError_t addWithCuda(int c[][SIZE], const int a[][SIZE], const int b[][SIZE], unsigned int size, dim3 threads)
{
    // changed the declaration of CUDA arrays to 2D matrices with one dimension size set
    int (*dev_a)[SIZE] = { 0 };
    int (*dev_b)[SIZE] = { 0 };
    int (*dev_c)[SIZE] = { 0 };
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // added the calculation of the matrix size
    const int dim = sqrt(size);
    // Added calculation of blocks number in both dimensions of the matrices
    dim3 blocks ((dim + threads.x - 1) / threads.x, (dim + threads.y - 1) / threads.y);

    //printf("%d\n", blocks);

    // Added block variable to addKernel function
    // Launch a kernel on the GPU with one thread for each element.
    // parametrized the kernel values with number and size of blocks
    addKernel<<<blocks, threads>>>(dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
    return cudaStatus;
}
