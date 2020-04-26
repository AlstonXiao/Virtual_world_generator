#include "../PerlinNoise.cuh"
#include <iostream>
#include <cstdlib>
using namespace std;

int 
main(){
    float p1x = 0;
    float p1y = 0;
    float p2x = 10;
    float p2y = 0;
    float p3x = 10;
    float p3y = 0;
    float p4x = 20;
    float p4y = 0;

    Vector2<float> controlP[4];
    controlP[0] = Vector2<float>(p1x,p1y);
    controlP[1] = Vector2<float>(p2x,p2y);
    controlP[2] = Vector2<float>(p3x,p3y);
    controlP[3] = Vector2<float>(p4x,p4y);
    // cout<<controlP[3];
    CanvasPoints* points = ProcessPoints(controlP);   
    ThreeDObject* obj = generateStreet(points);
    
    Generate(obj);
}