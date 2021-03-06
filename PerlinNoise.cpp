// Modified from Ken Perlin's implementation of the Perlin Noise
// https://mrl.nyu.edu/~perlin/noise/
#include <cmath>
#include "PerlinNoise.h"

#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif

using namespace std;
const int permutation[512] = { 151,160,137,91,90,15,
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

float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

float lerp(float t, float a, float b) { return a + t * (b - a); }

float grad(int hash, float x, float y, float z) {
    int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
    float u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
           v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

float noise(float x, float y, float z) {
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




float OctavePerlin(float x, float y, float z, int octaves, float persistence) {
    double addedtotal = 0;
    double frequency = 1;
    double amplitude = 1;
    double maxValue = 0;			// Used for normalizing result to 0.0 - 1.0
    for(int i=0;i<octaves;i++) {
        addedtotal += noise(x * frequency, y * frequency, z * frequency) * amplitude;
        
        maxValue += amplitude;
        
        amplitude *= persistence;
        frequency *= 2;
    }
    
    return addedtotal/maxValue;
}

void randomize(OutputObject Out) {
    size_t objectCount = Out.otherObjectCount + Out.streetCount;
  

    for (size_t i = 0; i < objectCount; ++i) {
        int octaves = i < Out.streetCount? 4 : 2;
        float persistence = i < Out.streetCount? 0.75 : 0.5;
        float divided = i < Out.streetCount? 20 : 40;
        for (size_t j = 0; j < Out.objects[i].vertice_count; j++){
            Out.objects[i].vertices[j].y += OctavePerlin(Out.objects[i].vertices[j].x, Out.objects[i].vertices[j].y, Out.objects[i].vertices[j].z, octaves, persistence)/divided;
        }
    }
}