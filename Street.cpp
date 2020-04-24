#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif

#ifndef STREET_H
#define STREET_H
#include "Street.h"
#endif

#ifndef GLOBAL_H
#define GLOBAL_H
#include "globals.h"
#endif

#include <algorithm>
#include <cmath>
#include <vector>
#include <ctime>

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
    // for (int i = 0; i <= PARAPOINTS; i++) {
    //     if (count > SECTIONSIZE) {
            
    //     }
    //     count += points[i].distance;
    // }   

    // merge the points together
    cout<<config;
    // send the Object to cuda for details randomization

    ThreeDObject obj = GenerateParts(points, 0, 1, config);
    return obj;
}

ThreeDObject GenerateParts(const CanvasPoints points[], const size_t start, const size_t end, const StreetConfig config){
    vector<Vector3<float>> vertices;
    vector<Vector3<int>> faces;
    float slopeDis = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.1));
    int vCount = 0;
    int vCountPre = 0;
    int vCountTotal = 0;
    for (size_t i = start; i < end; i++) {
        Vector2<float> fixedTangent = points[i].tan;
        Vector2<float> currentPosition = points[i].location;
        float currentDistance = 0;
        float paveDistance = 0;
        
        while (paveDistance < 5) {
            // most inner case 
            if(config.inner && currentDistance < config.innerWidth) {
                
                // edge condition
                if (currentDistance + DEFINITIONSTEP >= config.innerWidth) {
                    float tempDistance = config.innerWidth - currentDistance;
                    currentPosition += fixedTangent * tempDistance;
                    currentDistance += tempDistance;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.innerHeight, -currentPosition.y));
                    // upper slope
                    
                    currentDistance += slopeDis;
                    currentPosition += fixedTangent * slopeDis;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.centerHeight, -currentPosition.y));
                } else {
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                    config.innerHeight, -currentPosition.y ));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                }               
            } 

            // Central top
            else if(currentDistance < config.centerWidth) {
                // edge condition
                if (currentDistance + DEFINITIONSTEP >= config.centerWidth) {
                    float tempDistance = config.centerWidth - currentDistance;
                    currentPosition += fixedTangent * tempDistance;
                    currentDistance += tempDistance;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.centerHeight, -currentPosition.y));
                    // upper slope
                    currentDistance += slopeDis;
                    currentPosition += fixedTangent * slopeDis;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        0.0, -currentPosition.y));
                    paveDistance = 40;
                } else {
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                    config.centerHeight, -currentPosition.y ));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                }
                // next step
            }

            
        }

        // connecting the vertices using faces
        if (i != start) {
            vCountPre = vCount;
            vCount = vertices.size() - vCountTotal;
            int vCountPrePointer = vCountTotal - vCountPre;
            int vCountPointer = vCountTotal;
            vCountTotal = vertices.size();
            for (int j = 0; j < vCount - 1; j++) {
                faces.push_back(Vector3<int>(vCountPrePointer + j, vCountPointer + j, vCountPointer+1 + j));
                faces.push_back(Vector3<int>(vCountPrePointer + j, vCountPrePointer + 1 + j, vCountPointer+1 + j));
            }
        } else {
            vCount = vertices.size();
            vCountTotal = vCount;
        }
        

    }
    ThreeDObject returned;
    int verticeSize = vertices.size();
    int faceSize = faces.size();
    Vector3<float>* vertice = (Vector3<float>*)malloc(verticeSize*sizeof(Vector3<float>));
    Vector3<int>* face = (Vector3<int>*)malloc(verticeSize*sizeof(Vector3<int>));
    std::copy(vertices.begin(), vertices.end(), vertice);
    std::copy(faces.begin(), faces.end(), face);
    returned.vertices = vertice;
    returned.faces = face;
    returned.vertice_count = verticeSize;
    returned.face_count = faceSize;
    return returned;
}