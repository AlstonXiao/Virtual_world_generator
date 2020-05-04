#include "../PerlinNoise.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

int 
main(){

    ofstream objOutput("Perlin Test.obj");
    
    objOutput<<"o result\n";
    for (float i = 0; i < 10; i += 0.1) {
        for (float j = 0; j < 10; j += 0.1) {
            float noise = OctavePerlin(i,0,j, 3, 0.2) / 20;
            objOutput<<"v "<<i<<" "<<noise<<" "<<j<<"\n";
        }
    }
    
    objOutput.close();
}