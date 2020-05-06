#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif

#ifndef STREET_H
#define STREET_H
#include "Street.h"
#endif

#ifndef BEZIER_H
#define BEZIER_H
#include "Bezier.h"
#endif

#ifndef GENERATEIMAGE_H
#define GENERATEIMAGE_H
#include "GenerateImage.h"
#endif
#include "PerlinNoise.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
// The std::chrono namespace provides timer functions in C++
#include <chrono>

// std::ratio provides easy conversions between metric units
#include <ratio>
#include <algorithm>
// not needed for timers, provides std::pow function
#include <cmath>
#include <omp.h>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
int main(int argc, char** argv){
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    duration<double, std::milli> duration_sec;
    float p1x = stof(argv[1]);
    float p1y = stof(argv[2]);
    float p2x = stof(argv[3]);
    float p2y = stof(argv[4]);
    float p3x = stof(argv[5]);
    float p3y = stof(argv[6]);
    float p4x = stof(argv[7]);
    float p4y = stof(argv[8]);
    int GenerateOrNot = atoi(argv[9]);
    Vector2<float> controlP[4];
    unsigned int thread = atoi(argv[10]);

    if (thread > 20 || thread < 1) {
        fprintf(stderr, "number of thread is not in range 1 - 20");
        return 1;
    }
    omp_set_num_threads(thread);

    controlP[0] = Vector2<float>(p1x,p1y);
    controlP[1] = Vector2<float>(p2x,p2y);
    controlP[2] = Vector2<float>(p3x,p3y);
    controlP[3] = Vector2<float>(p4x,p4y);
    // cout<<controlP[3];

    OutputObject obj;
    // for (int i = 0; i < 5; i++) {
    //     CanvasPoints* points = ProcessPoints(controlP);   
    //     obj = generateStreet(points);
    //     randomize(obj);
    // }
    float totalTime = 0;
    for (int i = 0; i < 5; i++) {
        start = high_resolution_clock::now();
        CanvasPoints* points = ProcessPoints(controlP);   
        obj = generateStreet(points);
        end = high_resolution_clock::now();
        randomize(obj);
        duration_sec = std::chrono::duration_cast<duration<double, std::milli>>(end - start);
        totalTime += duration_sec.count();
    }
    cout<<"\nTime taken: "<<totalTime/5;
    
    if (GenerateOrNot == 1)
        Generate(obj);
}