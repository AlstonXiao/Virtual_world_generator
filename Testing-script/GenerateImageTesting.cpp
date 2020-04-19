#ifndef BEZIER_H
#define BEZIER_H
#include "../Bezier.h"
#endif

#ifndef DATATYPE_H
#define DATATYPE_H
#include "../DataType.h"
#endif

#ifndef GENERATEIMAGE_H
#define GENERATEIMAGE_H
#include "../GenerateImage.h"
#endif

#include <iostream>
#include <cstdlib>
using namespace std;
int 
main(){
    Vector3<float> controlP[4];
    controlP[0] = Vector3<float>(0.0,0.0,0.0);
    controlP[1] = Vector3<float>(100.0,0.0,0.0);
    controlP[2] = Vector3<float>(100.0,100.0,0.0);
    controlP[3] = Vector3<float>(0.0,100,0.0);

    Vector3<int> face[4];
    face[0] = Vector3<int>(0,1,2);
    face[1] = Vector3<int>(0,2,3);
    Generate(controlP, 4, face, 2);
}