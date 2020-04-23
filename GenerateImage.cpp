#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif

#ifndef GENERATEIMAGE_H
#define GENERATEIMAGE_H
#include "GenerateImage.h"
#endif

#include <iostream>
#include <fstream>
using namespace std;
void Generate(ThreeDObject OuputObject) {
    ofstream objOutput("output.obj");
    objOutput<<"o result\n";
    for (size_t i = 0; i < OuputObject.vertice_count; i++) {
        objOutput<<"v "<<OuputObject.vertices[i].x<<" "<<OuputObject.vertices[i].y<<" "<<OuputObject.vertices[i].z<<"\n";
    }
    objOutput<<"\n";
    for (size_t i = 0; i < OuputObject.face_count; i++) {
        objOutput<<"f "<<OuputObject.faces[i].x+1<<" "<<OuputObject.faces[i].y+1<<" "<<OuputObject.faces[i].z+1<<"\n";
    }
    objOutput.close();
}