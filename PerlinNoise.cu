// Modified from Ken Perlin's implementation of the Perlin Noise
// https://mrl.nyu.edu/~perlin/noise/
#include <cmath>
#include "PerlinNoise.cuh"
#include <iostream>

#ifndef GLOBAL_H
#define GLOBAL_H
#include "globals.h"
#endif

using namespace std;
const int Permutation[512] = { 151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
    151,160,137,91,90,15,
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

__device__ float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

__device__ float lerp(float t, float a, float b) { return a + t * (b - a); }

__device__ float grad(int hash, float x, float y, float z) {
    int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
    float u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
           v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

__device__ float noise(float x, float y, float z, int* permutation) {
    int X = (int)floor(x) & 255;                  // FIND UNIT CUBE THAT
    int Y = (int)floor(y) & 255;                  // CONTAINS POINT.
    int Z = (int)floor(z) & 255;
    x -= floor(x);                                // FIND RELATIVE X,Y,Z
    y -= floor(y);                                // OF POINT IN CUBE.
    z -= floor(z);
    float u = fade(x);                                // COMPUTE FADE CURVES
    float v = fade(y);                                // FOR EACH OF X,Y,Z.
    float w = fade(z);

    int A = permutation[X  ]+Y, AA = permutation[A]+Z, AB = permutation[A+1]+Z,      // HASH COORDINATES OF
        B = permutation[X+1]+Y, BA = permutation[B]+Z, BB = permutation[B+1]+Z;      // THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(permutation[AA  ], x  , y  , z   ),  // AND ADD
                                   grad(permutation[BA  ], x-1, y  , z   )), // BLENDED
                           lerp(u, grad(permutation[AB  ], x  , y-1, z   ),  // RESULTS
                                   grad(permutation[BB  ], x-1, y-1, z   ))),// FROM  8
                   lerp(v, lerp(u, grad(permutation[AA+1], x  , y  , z-1 ),  // CORNERS
                                   grad(permutation[BA+1], x-1, y  , z-1 )), // OF CUBE
                           lerp(u, grad(permutation[AB+1], x  , y-1, z-1 ),
                                   grad(permutation[BB+1], x-1, y-1, z-1 ))));
 }



__global__ void PerlinKernel(Vector3<float>* points, const int* permu, const size_t size, const int octaves, const float persistence, const float divided) {
    __shared__ int permutation[512];
    unsigned int pos = blockIdx.x * blockDim.x + threadIdx.x;
    if (threadIdx.x < 512) permutation[threadIdx.x] = permu[threadIdx.x];
    __syncthreads();
    float addedtotal = 0;
    float frequency = 1;
    float amplitude = 1;
    float maxValue = 0;
    float x = points[pos].x;
    float y = points[pos].y;
    float z = points[pos].z;
    if (pos < size){
        for(int i=0;i<octaves;i++) {
            addedtotal += noise(x * frequency, y * frequency, z * frequency, permutation) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }
        points[pos].y += addedtotal/maxValue / divided;
    }
}
__host__ void randomize(OutputObject Out) {
    size_t objectCount = Out.otherObjectCount + Out.streetCount;
    // cout<<"now in CUDA, with "<<objectCount<<" objects\n" << std::flush;
    Vector3<float> **host_vertex = new Vector3<float>*[objectCount];
    Vector3<float> **device_vertex = new Vector3<float>*[objectCount];
    cudaStream_t stream[objectCount]; 
    // cout<<"allocate done\n" << std::flush;
    int *permu;
    // cudaError_t code;
    cudaMallocManaged(&permu, 512*sizeof(int));
    // cout<<cudaGetErrorString(code);
    // cout<<Permutation[10]<<"\n"<< std::flush;
    // cout<<"stg"<< std::flush;
    for (int i = 0; i < 512; i++) {
        permu[i] = Permutation[i];
        // cout<<permu[i]<<"\n"<< std::flush;
    }
    // cout<<permu[2]<<" "<<permu[511]<<"\n"<< std::flush;
    // cout<<Out.objects[0].vertice_count<<"\n"<< std::flush;
    for(size_t i = 0; i < objectCount; ++i) {
        cudaStreamCreate(&stream[i]);
        cudaHostAlloc(&host_vertex[i], Out.objects[i].vertice_count * sizeof(Vector3<float>), cudaHostAllocDefault);
        cudaMalloc(&device_vertex[i],  Out.objects[i].vertice_count * sizeof(Vector3<float>));
        for (size_t j = 0; j < Out.objects[i].vertice_count; j++){
            host_vertex[i][j] = Out.objects[i].vertices[j];
        }
    }
    // cout<<host_vertex[0][2]<<" "<<host_vertex[0][9999]<<"\n"<< std::flush;;

    for (size_t i = 0; i < objectCount; ++i) {
        cudaMemcpyAsync( device_vertex[i], host_vertex[i], Out.objects[i].vertice_count * sizeof(Vector3<float>), cudaMemcpyHostToDevice, stream[i]);
        unsigned int blockNum = (Out.objects[i].vertice_count + THREAD_PER_BLOCK - 1) / THREAD_PER_BLOCK;
        int octaves = i < Out.streetCount? 4 : 2;
        float persistence = i < Out.streetCount? 0.75 : 0.5;
        float divided = i < Out.streetCount? 20 : 40;
        PerlinKernel<<<blockNum, THREAD_PER_BLOCK, 0, stream[i]>>>(device_vertex[i],permu, Out.objects[i].vertice_count, octaves, persistence, divided);
        cudaMemcpyAsync( host_vertex[i], device_vertex[i], Out.objects[i].vertice_count * sizeof(Vector3<float>), cudaMemcpyDeviceToHost, stream[i]);
    }
    for(size_t i = 0; i < objectCount; ++i) cudaStreamSynchronize( stream[i]);
    // cout<<host_vertex[0][2]<<" "<<host_vertex[0][9999]<<"\n"<< std::flush;;

    for(size_t i = 0; i < objectCount; ++i) {
        for (size_t j = 0; j < Out.objects[i].vertice_count; j++){
            Out.objects[i].vertices[j] = host_vertex[i][j];
        }  
        cudaStreamDestroy(stream[i]);
        cudaFreeHost(host_vertex[i]);
        cudaFree(device_vertex[i]);
    }
}