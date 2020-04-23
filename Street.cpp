#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif

#ifndef STREET_H
#define STREET_H
#include "Street.h"
#endif

#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

StreetConfig generateRandomConfig(){
    srand (static_cast <unsigned> (time(0)));
    StreetConfig config;
    config.centerWidth =  0.3 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 2.7));
    config.centerHeight =  0.4 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.4));


    config.inner = rand() < RAND_MAX / 2;
    if (config.inner) {
        config.innerWidth = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((config.centerWidth - 0.3)/2.0)));
        config.innerHeight = config.centerHeight - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.4));
    } else {
        config.innerWidth = -1;
        config.innerHeight = -1;
    }
    
    config.laneCount = rand() % 3 + 1;
    config.laneWidth =  2.7 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 2));

    config.laneDivide = rand() < RAND_MAX / 4;
    if (config.laneDivide) {
        config.laneDiverHeight = 0.1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.4));
    } else {
        config.laneDiverHeight = -1;
    }

    config.shoulderWidth = config.laneWidth - static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
    config.paveHeight = 0.1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.4));
    return config;
}

StreetConfigExtra generateRandomConfigExtra(){
    srand (static_cast <unsigned> (time(0)));
    StreetConfigExtra config;
    config.extraSpace = rand() < RAND_MAX / 10;
    if (config.extraSpace) {
        config.extraSpaceWidth = 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 2));
        config.extraSpaceDistance = 3 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 4));
    } else {
        config.extraSpaceWidth = -1;
        config.extraSpaceDistance = -1;
    }
    return config;
}

ThreeDObject generateStreet(CanvasPoints points[]){
    
    // Generate random config
    StreetConfig config = generateRandomConfig();

    // arc-length to 10 meters segment
    // send to OpenMP to generate the streets, each part should determine their own extraSpace
    // #pragma omp firstprivate
    float count = 0;
    int previous = 0;
    for (int i = 0; i <= PARAPOINTS; i++) {
        if (count > SECTIONSIZE) {
            
        }
        count += points[i].distance;
    }   


    
    // merge the points together
    cout<<config;
    // send the Object to cuda for details randomization

    ThreeDObject obj;
    return obj;
}

ThreeDObject GenerateParts(const CanvasPoints points[], const size_t start, const size_t end, const StreetConfig config){
    vector<Vector3<float>> vertices;
    vector<Vector3<int>> faces;
    float currentDistance = 0;
    
    for (size_t i = start; i < end; i++) {
        vector2<float> fixedTangent = points[i].tan * DEFINITIONSTEP;
        // most inner case 
        if(config.inner && currentDistance < config.innerWidth) {
            
            vertices.push_back(Vector3<float>(points[i].location.x, config.innerHeight, -points[i].location.y));
            if (currentDistance + DEFINITIONSTEP >= config.innerWidth) {
                vertices.push_back(Vector3<float>(points[i].location.x, config.innerHeight, -points[i].location.y));
            }
        }
    }
}