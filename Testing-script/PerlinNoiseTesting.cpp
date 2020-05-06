#include "../PerlinNoise.cuh"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

int 
main(){
    OutputObject Out;
    Out.streetCount = 1;
    Out.otherObjectCount = 0;
    Vector3<float> *vertice = new Vector3<float>[10000];
    for (int i = 0; i < 100; i += 1) {
        for (int j = 0; j < 100; j += 1) {
            vertice[i*100+j].x = (float)i/10;
            vertice[i*100+j].y = 0;
            vertice[i*100+j].z = (float)j/10;
        }
    }
    ThreeDObject three;
    three.vertices = vertice;
    three.vertice_count = 10000;
    Out.objects = &three;
    cout<<"inputing\n";
    randomize(Out);

    ofstream objOutput("PerlinTest.obj");
    cout<<"outputing\n";
    objOutput<<"o result\n";
    for (int i = 0; i < 100; i += 1) {
        for (int j = 0; j < 100; j += 1) {
            objOutput<<"v "<<vertice[i*100+j].x<<" "<<vertice[i*100+j].y<<" "<<vertice[i*100+j].z<<"\n";
        }
    }
    
    objOutput.close();
}