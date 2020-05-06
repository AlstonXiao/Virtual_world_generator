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
void Generate(OutputObject Out) {
    ofstream objOutput("output.obj");
    ThreeDObject* OuputObject = Out.objects;
    size_t numberOfObject = Out.otherObjectCount + Out.streetCount;
    objOutput<<"o result\n";
    for (size_t j = 0; j < numberOfObject; j++) {
        for (size_t i = 0; i < OuputObject[j].vertice_count; i++) {
            objOutput<<"v "<<OuputObject[j].vertices[i].x<<" "<<OuputObject[j].vertices[i].y<<" "<<OuputObject[j].vertices[i].z<<"\n";
        }
    }


    objOutput<<"\n";
    int preFace = 1;
    for (size_t j = 0; j < numberOfObject; j++) {
        for (size_t i = 0; i < OuputObject[j].face_count; i++) {
            objOutput<<"f "<<OuputObject[j].faces[i].x+preFace<<" "<<OuputObject[j].faces[i].y+preFace<<" "<<OuputObject[j].faces[i].z+preFace<<"\n";
        }
        preFace+=OuputObject[j].vertice_count;
    }
    objOutput<<"\n";
    // cout<<"obj vcout:"<<OuputObject[0].vertice_count<<" Obj2 vount:"<<OuputObject[1].vertice_count<<"\n";    

    objOutput.close();
}