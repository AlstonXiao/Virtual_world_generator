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

using namespace std;
int main(int argc, char** argv){
    float p1x = stof(argv[1]);
    float p1y = stof(argv[2]);
    float p2x = stof(argv[3]);
    float p2y = stof(argv[4]);
    float p3x = stof(argv[5]);
    float p3y = stof(argv[6]);
    float p4x = stof(argv[7]);
    float p4y = stof(argv[8]);

    Vector2<float> controlP[4];
    controlP[0] = Vector2<float>(p1x,p1y);
    controlP[1] = Vector2<float>(p2x,p2y);
    controlP[2] = Vector2<float>(p3x,p3y);
    controlP[3] = Vector2<float>(p4x,p4y);
    // cout<<controlP[3];
    CanvasPoints* points = ProcessPoints(controlP);   
    OutputObject obj = generateStreet(points);
    Generate(obj);
}