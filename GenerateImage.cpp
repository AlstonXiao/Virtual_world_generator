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
void Generate(ThreeDObject OuputObject[]) {
    ofstream objOutput("output.obj");
    
    objOutput<<"o result\n";
    for (size_t i = 0; i < OuputObject[0].vertice_count; i++) {
        objOutput<<"v "<<OuputObject[0].vertices[i].x<<" "<<OuputObject[0].vertices[i].y<<" "<<OuputObject[0].vertices[i].z<<"\n";
    }
    for (size_t i = 0; i < OuputObject[1].vertice_count; i++) {
        objOutput<<"v "<<OuputObject[1].vertices[i].x<<" "<<OuputObject[1].vertices[i].y<<" "<<OuputObject[1].vertices[i].z<<"\n";
    }
    objOutput<<"\n";
    for (size_t i = 0; i < OuputObject[0].face_count; i++) {
        objOutput<<"f "<<OuputObject[0].faces[i].x+1<<" "<<OuputObject[0].faces[i].y+1<<" "<<OuputObject[0].faces[i].z+1<<"\n";
    }
    objOutput<<"\n";
    cout<<"obj vcout:"<<OuputObject[0].vertice_count<<" Obj2 vount:"<<OuputObject[1].vertice_count<<"\n";
    for (size_t i = 0; i < OuputObject[1].face_count; i++) {
        objOutput<<"f "<<OuputObject[1].faces[i].x+1 +OuputObject[0].vertice_count <<" "<<OuputObject[1].faces[i].y+1+OuputObject[0].vertice_count<<" "<<OuputObject[1].faces[i].z+1+OuputObject[0].vertice_count<<"\n";
    }
    objOutput.close();
}