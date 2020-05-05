#include "../PerlinNoise.cuh"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

int 
main(){

    ofstream objOutput("Perlin Test.obj");
    
    objOutput<<"o result\n";
    for (float i = 0; i < 20; i += 0.1) {
        for (float j = 0; j < 20; j += 0.1) {
            float noise = OctavePerlin(i,0,j, 1, 0.5);
            objOutput<<"v "<<i<<" "<<noise<<" "<<j<<"\n";
        }
    }
    
    objOutput.close();
}