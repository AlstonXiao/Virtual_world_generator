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
void Generate(Vector3<float> vertices[], size_t vertice_count, Vector3<int> faces[], size_t face_count) {
    ofstream objOutput("output.obj");
    objOutput<<"o result\n";
    for (size_t i = 0; i < vertice_count; i++) {
        objOutput<<"v "<<vertices[i].x<<" "<<vertices[i].y<<" "<<vertices[i].z<<"\n";
    }
    objOutput<<"\n";
    for (size_t i = 0; i < face_count; i++) {
        objOutput<<"f "<<faces[i].x+1<<" "<<faces[i].y+1<<" "<<faces[i].z+1<<"\n";
    }
    objOutput.close();
}