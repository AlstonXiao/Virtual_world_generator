#ifndef BEZIER_H
#define BEZIER_H
#include "Bezier.h"
#endif

#ifndef GLOBALS_H
#define GLOBALS_H
#include "globals.h"
#endif

#include <cmath>
using namespace std;
CanvasPoints* ProcessPoints(Vector2<float>* controllPoints) {
    Vector2<float> startPoint = controllPoints[0];
    
    Vector2<float> endPoint = controllPoints[3];
    // cout<<endPoint<<"\n";
    Vector2<float> startDerivation = (controllPoints[1] - controllPoints[0]) * 3;
    Vector2<float> endDerivation = (controllPoints[3] - controllPoints[2]) * 3;
    TwoD_function curve(startPoint, startDerivation, startPoint*(-3)+startDerivation*(-2)+endPoint*3-endDerivation,  startPoint*(2)+startDerivation+endPoint*(-2)+endDerivation);
    CanvasPoints* points = new CanvasPoints[PARAPOINTS+1];

    for (int i = 0; i <= PARAPOINTS; i++) {
        float u = ((float) i) / ((float)PARAPOINTS);
        points[i].location = curve.at(u);
        points[i].tan = Vector2<float>(-curve.dirvAt(u).y, curve.dirvAt(u).x);
        float tan_d = sqrt(points[i].tan.x *points[i].tan.x  + points[i].tan.y *points[i].tan.y);
        points[i].tan = points[i].tan / tan_d;
        // cout<<points[i].location<<points[i].tan<<"\n";
    }

    for (int i = 0; i < PARAPOINTS; i++) {
        points[i].distance = distance(points[i].location, points[i+1].location);
    }
    return points;
} 
