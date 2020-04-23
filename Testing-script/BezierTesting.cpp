#ifndef BEZIER_H
#define BEZIER_H
#include "../Bezier.h"
#endif
#ifndef GLOBALS_H
#define GLOBALS_H
#include "../globals.h"
#endif
#ifndef DATATYPE_H
#define DATATYPE_H
#include "../DataType.h"
#endif

#include <iostream>
#include <cstdlib>
using namespace std;
int 
main(){
    Vector2<float> controlP[4];
    controlP[0] = Vector2<float>(0.0,0.0);
    controlP[1] = Vector2<float>(100.0,0.0);
    controlP[2] = Vector2<float>(400.0,0.0);
    controlP[3] = Vector2<float>(500.0,0.0);
    // cout<<controlP[3];
    CanvasPoints* points = ProcessPoints(controlP);
    for (int i = 0; i <= PARAPOINTS; i++){
        cout<<points[i].location<<" "<<points[i].tan<<" "<<points[i].distance<<"\n";
    }
}