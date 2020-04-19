#ifndef BEZIER_H
#define BEZIER_H
#include "Bezier.h"
#endif

#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif

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
        points[i].tan = curve.dirvAt(u);
    }
    return points;
} 
