#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif

#ifndef STREET_H
#define STREET_H
#include "Street.h"
#endif

#ifndef GLOBAL_H
#define GLOBAL_H
#include "globals.h"
#endif

#include "omp.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <ctime>

using namespace std;
StreetConfig generateRandomConfig(){
    srand (static_cast <unsigned> (time(0)));
    StreetConfig config;
    config.centerWidth =  0.3 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 2.7));
    config.centerHeight =  0.8 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.4));


    config.inner = rand() < RAND_MAX / 2;
    if (config.inner) {
        config.innerWidth = 0.1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((config.centerWidth - 0.3)/2.0)));
        config.innerHeight = config.centerHeight - static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.4));
    } else {
        config.innerWidth = -1;
        config.innerHeight = -1;
    }
    
    config.laneCount = rand() % 3 + 1;
    config.laneWidth =  2.7 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 2));

    config.laneDivide = rand() < RAND_MAX / 4;
    if (config.laneDivide) {
        config.laneDiverHeight = 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.4));
        config.laneDivderWidth = 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.8));
    } else {
        config.laneDiverHeight = -1;
        config.laneDivderWidth = -1;
    }

    config.shoulderWidth = config.laneWidth - static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
    config.paveHeight = 0.1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.4));
    return config;
}

StreetConfigExtra generateRandomConfigExtra(){
    srand (static_cast <unsigned> (time(0)));
    StreetConfigExtra config;
    config.extraSpace = rand() < RAND_MAX / 10;
    if (config.extraSpace) {
        config.extraSpaceWidth = 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 2));
        config.extraSpaceDistance = 3 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 4));
    } else {
        config.extraSpaceWidth = -1;
        config.extraSpaceDistance = -1;
    }
    return config;
}

OutputObject generateStreet(CanvasPoints points[]){
    
    // Generate random config
    StreetConfig config = generateRandomConfig();
    // cout<<config<<std::flush;
    int totalpartitions = (PARAPOINTS + SECTIONSIZE) / SECTIONSIZE * 2; // * 2 for each side
    int totalObjects = totalpartitions + config.laneCount - 1 + 2 + 1; //lanecount - 1 is the line in the middle, 2 for line at the side, final 1 is for the bars.
    int finalObjectsStart = totalpartitions + config.laneCount - 1;
    ThreeDObject* obj = new ThreeDObject[totalObjects];

    float linewitdth = 0.1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.1));
    float barspos = config.centerWidth + config.laneWidth * config.laneCount + 0.3 + config.shoulderWidth + (config.laneDivide ? config.laneDivderWidth : 0);
    float slopeDis = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.1));
    // send to OpenMP to generate the streets
    #pragma omp parallel 
    {
        #pragma omp single nowait
        {   
            int i = 0;
            while(i*SECTIONSIZE + SECTIONSIZE <= PARAPOINTS + 1) {
                if (i == 0) {
                    #pragma omp task
                    obj[2 * i] = GenerateParts(points, SECTIONSIZE * i, SECTIONSIZE * (i + 1), config, 0, slopeDis);
                    // cout<<2 * i<<"\n";
                    #pragma omp task
                    obj[2 * i + 1] = GenerateParts(points, SECTIONSIZE * i, SECTIONSIZE * (i + 1), config, 1, slopeDis);
                    // cout<<2 * i + 1<<"\n";
                } else {
                    #pragma omp task
                    obj[2 * i] = GenerateParts(points, SECTIONSIZE * i - 1, SECTIONSIZE * (i + 1), config, 0, slopeDis);
                    // cout<<2 * i<<"\n";
                    #pragma omp task
                    obj[2 * i + 1] = GenerateParts(points, SECTIONSIZE * i - 1, SECTIONSIZE * (i + 1), config, 1, slopeDis);
                    // cout<<2 * i + 1<<"\n";
                }
                i ++;
            }

            if (i != PARAPOINTS + 1) {
                #pragma omp task
                obj[2 * i] = GenerateParts(points, SECTIONSIZE * i - 1, PARAPOINTS + 1, config, 0, slopeDis);
                // cout<<2 * i<<"\n";
                #pragma omp task
                obj[2 * i + 1] = GenerateParts(points, SECTIONSIZE * i - 1, PARAPOINTS + 1, config, 1, slopeDis);
                // cout<<2 * i + 1<<"\n";
            }

            
            for (int i = 0; i < config.laneCount - 1; i++) {
                float position = config.centerWidth + (i+1) * config.laneWidth;
                #pragma omp task
                obj[totalpartitions+i] = GenerateSignDashed(points, position, linewitdth);
                // cout<<totalpartitions+i<<"\n";
            }
            #pragma omp task
            obj[finalObjectsStart] = GenerateSign(points, config.centerWidth + linewitdth + 0.25, linewitdth);
            // cout<<finalObjectsStart<<"\n";
            #pragma omp task
            obj[finalObjectsStart+1] = GenerateSign(points, config.centerWidth + config.laneWidth * config.laneCount - linewitdth - 0.25, linewitdth);
            #pragma omp task
            obj[finalObjectsStart+2] = GenerateBars(points, barspos);
            
        }

    }
 
    // merge the points together       
    cout<<"all done "<<"\n"<<std::flush;

    // Returned logic
    OutputObject out;
    out.objects = obj;
    out.streetCount = totalpartitions;
    out.otherObjectCount = totalObjects - totalpartitions;

    return out;
}

ThreeDObject GenerateBars(const CanvasPoints points[], const float position){
    vector<Vector3<float>> vertices;
    vector<Vector3<int>> faces;
    float counter = 0;
    int currentPoint = 0;
    int vCount = 0;
    float height = 0.4 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
    float dimension = 0.05 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX / 0.05));
    Vector2<float> fixedTangent = points[currentPoint].tan;
    Vector2<float> currentPosition = points[currentPoint].location;
    Vector2<float> center = currentPosition + fixedTangent *position ;
    vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height-dimension, -(center+fixedTangent*dimension).y));
    vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height+dimension, -(center+fixedTangent*dimension).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height+dimension, -(center-fixedTangent*dimension).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height-dimension, -(center-fixedTangent*dimension).y));

    center = currentPosition - fixedTangent *position ;
    vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height-dimension, -(center+fixedTangent*dimension).y));
    vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height+dimension, -(center+fixedTangent*dimension).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height+dimension, -(center-fixedTangent*dimension).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height-dimension, -(center-fixedTangent*dimension).y));

    faces.push_back(Vector3<int>(vCount, vCount + 3, vCount+2));
    faces.push_back(Vector3<int>(vCount,vCount + 2,  vCount+1));

    faces.push_back(Vector3<int>(vCount+4, vCount + 7, vCount+6));
    faces.push_back(Vector3<int>(vCount+4, vCount +6, vCount+5));

    while (currentPoint < PARAPOINTS + 1) {
        Vector2<float> fixedTangent = points[currentPoint].tan;
        Vector2<float> currentPosition = points[currentPoint].location;
        Vector2<float> center = currentPosition + fixedTangent *position ;
        vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height-dimension, -(center+fixedTangent*dimension).y));
        vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height+dimension, -(center+fixedTangent*dimension).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height+dimension, -(center-fixedTangent*dimension).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height-dimension, -(center-fixedTangent*dimension).y));

        center = currentPosition - fixedTangent *position ;
        vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height-dimension, -(center+fixedTangent*dimension).y));
        vertices.push_back(Vector3<float>((center+fixedTangent*dimension).x, height+dimension, -(center+fixedTangent*dimension).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height+dimension, -(center-fixedTangent*dimension).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*dimension).x, height-dimension, -(center-fixedTangent*dimension).y));

        faces.push_back(Vector3<int>(vCount, vCount + 1, vCount+8));
        faces.push_back(Vector3<int>(vCount + 1,vCount + 9,  vCount+8));

        faces.push_back(Vector3<int>(vCount+1, vCount + 2, vCount+9));
        faces.push_back(Vector3<int>(vCount+2, vCount +10, vCount+9));

        faces.push_back(Vector3<int>(vCount+2, vCount + 3, vCount+10));
        faces.push_back(Vector3<int>(vCount+3, vCount +11, vCount+10));

        faces.push_back(Vector3<int>(vCount+3, vCount + 0, vCount+11));
        faces.push_back(Vector3<int>(vCount+0, vCount +8, vCount+11));

        faces.push_back(Vector3<int>(vCount+4, vCount + 5, vCount+12));
        faces.push_back(Vector3<int>(vCount+5, vCount +13, vCount+12));

        faces.push_back(Vector3<int>(vCount+5, vCount + 6, vCount+13));
        faces.push_back(Vector3<int>(vCount+6, vCount +14, vCount+13));

        faces.push_back(Vector3<int>(vCount+6, vCount + 7, vCount+14));
        faces.push_back(Vector3<int>(vCount+7, vCount +15, vCount+14));

        faces.push_back(Vector3<int>(vCount+7, vCount + 4, vCount+15));
        faces.push_back(Vector3<int>(vCount+4, vCount + 12, vCount+15));
        vCount += 8;
        currentPoint++;        
    }
    faces.push_back(Vector3<int>(vCount, vCount + 2, vCount+3));
    faces.push_back(Vector3<int>(vCount,vCount + 1,  vCount+2));

    faces.push_back(Vector3<int>(vCount+4, vCount + 6, vCount+7));
    faces.push_back(Vector3<int>(vCount+4, vCount + 5, vCount+6));
    vCount += 8;
    currentPoint = 0;
    while (currentPoint < PARAPOINTS + 1) {
        if (counter >= BARDISTANCE) {
            Vector2<float> fixedTangent = points[currentPoint].tan;
            Vector2<float> currentPosition = points[currentPoint].location;
            center = currentPosition + fixedTangent *position ;
            vertices.push_back(Vector3<float>(center.x - dimension/2, height, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension/2, height, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension, height, -center.y));
            vertices.push_back(Vector3<float>(center.x + dimension/2, height, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension/2, height, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension, height, -center.y));

            vertices.push_back(Vector3<float>(center.x - dimension/2, 0, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension/2, 0, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension, 0,-center.y));
            vertices.push_back(Vector3<float>(center.x + dimension/2, 0, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension/2, 0, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension, 0, -center.y));
            faces.push_back(Vector3<int>(vCount, vCount + 1, vCount+6));
            faces.push_back(Vector3<int>(vCount + 1,vCount + 7,  vCount+6));

            faces.push_back(Vector3<int>(vCount + 1, vCount + 2, vCount+7));
            faces.push_back(Vector3<int>(vCount + 2, vCount + 8, vCount+7));

            faces.push_back(Vector3<int>(vCount + 2, vCount + 3, vCount+8));
            faces.push_back(Vector3<int>(vCount + 3, vCount + 9, vCount+8));
            
            faces.push_back(Vector3<int>(vCount + 3, vCount + 4, vCount+9));
            faces.push_back(Vector3<int>(vCount + 4, vCount + 10, vCount+9));

            faces.push_back(Vector3<int>(vCount + 4, vCount + 5, vCount+10));
            faces.push_back(Vector3<int>(vCount + 5, vCount + 11, vCount+10));

            faces.push_back(Vector3<int>(vCount + 5, vCount + 0, vCount+11));
            faces.push_back(Vector3<int>(vCount + 0, vCount + 6, vCount+11));

            faces.push_back(Vector3<int>(vCount + 5, vCount + 1, vCount+0));
            faces.push_back(Vector3<int>(vCount + 5, vCount + 2, vCount+1));

            faces.push_back(Vector3<int>(vCount + 5, vCount + 3, vCount+2));
            faces.push_back(Vector3<int>(vCount + 5, vCount + 4, vCount+3));
            vCount +=12;
            center = currentPosition - fixedTangent *position ;
            vertices.push_back(Vector3<float>(center.x - dimension/2, height, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension/2, height, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension, height, -center.y));
            vertices.push_back(Vector3<float>(center.x + dimension/2, height, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension/2, height, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension, height, -center.y));
            vertices.push_back(Vector3<float>(center.x - dimension/2, 0, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension/2, 0, -(center.y + dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x + dimension, 0,-center.y));
            vertices.push_back(Vector3<float>(center.x + dimension/2, 0, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension/2, 0, -(center.y - dimension*sqrt(3)/2)));
            vertices.push_back(Vector3<float>(center.x - dimension, 0, -center.y));
            faces.push_back(Vector3<int>(vCount, vCount + 1, vCount+6));
            faces.push_back(Vector3<int>(vCount + 1,vCount + 7,  vCount+6));

            faces.push_back(Vector3<int>(vCount + 1, vCount + 2, vCount+7));
            faces.push_back(Vector3<int>(vCount + 2, vCount + 8, vCount+7));

            faces.push_back(Vector3<int>(vCount + 2, vCount + 3, vCount+8));
            faces.push_back(Vector3<int>(vCount + 3, vCount + 9, vCount+8));
            
            faces.push_back(Vector3<int>(vCount + 3, vCount + 4, vCount+9));
            faces.push_back(Vector3<int>(vCount + 4, vCount + 10, vCount+9));

            faces.push_back(Vector3<int>(vCount + 4, vCount + 5, vCount+10));
            faces.push_back(Vector3<int>(vCount + 5, vCount + 11, vCount+10));

            faces.push_back(Vector3<int>(vCount + 5, vCount + 0, vCount+11));
            faces.push_back(Vector3<int>(vCount + 0, vCount + 6, vCount+11));

            faces.push_back(Vector3<int>(vCount + 5, vCount + 1, vCount+0));
            faces.push_back(Vector3<int>(vCount + 5, vCount + 2, vCount+1));

            faces.push_back(Vector3<int>(vCount + 5, vCount + 3, vCount+2));
            faces.push_back(Vector3<int>(vCount + 5, vCount + 4, vCount+3));
            vCount +=12;
            counter = 0;            

        }
        counter += points[currentPoint].distance;
        currentPoint ++;
    }
    ThreeDObject returned;
    int verticeSize = vertices.size();
    int faceSize = faces.size();
    Vector3<float>* vertice = new Vector3<float>[verticeSize];
    Vector3<int>* face = new Vector3<int>[faceSize];
    std::copy(vertices.begin(), vertices.end(), vertice);
    std::copy(faces.begin(), faces.end(), face);
    returned.vertices = vertice;
    returned.faces = face;
    returned.vertice_count = verticeSize;
    returned.face_count = faceSize;
    return returned;
}



ThreeDObject GenerateSign(const CanvasPoints points[], const float position, const float linewitdth){
    vector<Vector3<float>> vertices;
    vector<Vector3<int>> faces;
    int currentPoint = 0;
    int vCount = 0;
    

    Vector2<float> fixedTangent = points[currentPoint].tan;
    Vector2<float> currentPosition = points[currentPoint].location;
    Vector2<float> center = currentPosition + fixedTangent *position ;
    vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
    vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.02, -(center+fixedTangent*linewitdth).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.02, -(center-fixedTangent*linewitdth).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

    center = currentPosition - fixedTangent *position ;
    vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
    vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.02, -(center+fixedTangent*linewitdth).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.02, -(center-fixedTangent*linewitdth).y));
    vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

    faces.push_back(Vector3<int>(vCount, vCount + 3, vCount+2));
    faces.push_back(Vector3<int>(vCount,vCount + 2,  vCount+1));

    faces.push_back(Vector3<int>(vCount+4, vCount + 7, vCount+6));
    faces.push_back(Vector3<int>(vCount+4, vCount +6, vCount+5));
    while (currentPoint < PARAPOINTS + 1) {
      
        Vector2<float> fixedTangent = points[currentPoint].tan;
        Vector2<float> currentPosition = points[currentPoint].location;
        Vector2<float> center = currentPosition + fixedTangent *position ;
        vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
        vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.05, -(center+fixedTangent*linewitdth).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.05, -(center-fixedTangent*linewitdth).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

        center = currentPosition - fixedTangent *position ;
        vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
        vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.05, -(center+fixedTangent*linewitdth).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.05, -(center-fixedTangent*linewitdth).y));
        vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

        faces.push_back(Vector3<int>(vCount, vCount + 1, vCount+8));
        faces.push_back(Vector3<int>(vCount + 1,vCount + 9,  vCount+8));

        faces.push_back(Vector3<int>(vCount+1, vCount + 2, vCount+9));
        faces.push_back(Vector3<int>(vCount+2, vCount +10, vCount+9));

        faces.push_back(Vector3<int>(vCount+2, vCount + 3, vCount+10));
        faces.push_back(Vector3<int>(vCount+3, vCount +11, vCount+10));

        faces.push_back(Vector3<int>(vCount+4, vCount + 5, vCount+12));
        faces.push_back(Vector3<int>(vCount+5, vCount +13, vCount+12));

        faces.push_back(Vector3<int>(vCount+5, vCount + 6, vCount+13));
        faces.push_back(Vector3<int>(vCount+6, vCount +14, vCount+13));

        faces.push_back(Vector3<int>(vCount+6, vCount + 7, vCount+14));
        faces.push_back(Vector3<int>(vCount+7, vCount +15, vCount+14));
        vCount += 8;
        currentPoint++;
           
    }
    faces.push_back(Vector3<int>(vCount, vCount + 2, vCount+3));
    faces.push_back(Vector3<int>(vCount,vCount + 1,  vCount+2));

    faces.push_back(Vector3<int>(vCount+4, vCount + 6, vCount+7));
    faces.push_back(Vector3<int>(vCount+4, vCount + 5, vCount+6));

    ThreeDObject returned;
    int verticeSize = vertices.size();
    int faceSize = faces.size();
    Vector3<float>* vertice = new Vector3<float>[verticeSize];
    Vector3<int>* face = new Vector3<int>[faceSize];
    std::copy(vertices.begin(), vertices.end(), vertice);
    std::copy(faces.begin(), faces.end(), face);
    returned.vertices = vertice;
    returned.faces = face;
    returned.vertice_count = verticeSize;
    returned.face_count = faceSize;
    return returned;
}


ThreeDObject GenerateSignDashed(const CanvasPoints points[], const float position, const float linewitdth){
    vector<Vector3<float>> vertices;
    vector<Vector3<int>> faces;
    float counter = LANEDASH;
    bool dashed = false ;
    int currentPoint = 0;
    int vCount = 0;
    
    while (currentPoint < PARAPOINTS + 1) {
        if (counter < LANEDASH) {
            if (!dashed) {
                currentPoint++;
                counter += points[currentPoint].distance;
            } else {
                Vector2<float> fixedTangent = points[currentPoint].tan;
                Vector2<float> currentPosition = points[currentPoint].location;
                Vector2<float> center = currentPosition + fixedTangent *position ;
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.05, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.05, -(center-fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

                center = currentPosition - fixedTangent *position ;
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.05, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.05, -(center-fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

                faces.push_back(Vector3<int>(vCount, vCount + 1, vCount+8));
                faces.push_back(Vector3<int>(vCount + 1,vCount + 9,  vCount+8));

                faces.push_back(Vector3<int>(vCount+1, vCount + 2, vCount+9));
                faces.push_back(Vector3<int>(vCount+2, vCount +10, vCount+9));

                faces.push_back(Vector3<int>(vCount+2, vCount + 3, vCount+10));
                faces.push_back(Vector3<int>(vCount+3, vCount +11, vCount+10));

                faces.push_back(Vector3<int>(vCount+4, vCount + 5, vCount+12));
                faces.push_back(Vector3<int>(vCount+5, vCount +13, vCount+12));

                faces.push_back(Vector3<int>(vCount+5, vCount + 6, vCount+13));
                faces.push_back(Vector3<int>(vCount+6, vCount +14, vCount+13));

                faces.push_back(Vector3<int>(vCount+6, vCount + 7, vCount+14));
                faces.push_back(Vector3<int>(vCount+7, vCount +15, vCount+14));
                vCount += 8;
                currentPoint++;
                counter += points[currentPoint].distance;
            }   
        } else {
            counter = 0;
            if (!dashed){
                Vector2<float> fixedTangent = points[currentPoint].tan;
                Vector2<float> currentPosition = points[currentPoint].location;
                Vector2<float> center = currentPosition + fixedTangent *position ;
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.02, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.02, -(center-fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

                center = currentPosition - fixedTangent *position ;
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, -1, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center+fixedTangent*linewitdth).x, 0.02, -(center+fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, 0.02, -(center-fixedTangent*linewitdth).y));
                vertices.push_back(Vector3<float>((center-fixedTangent*linewitdth).x, -1, -(center-fixedTangent*linewitdth).y));

                faces.push_back(Vector3<int>(vCount, vCount + 3, vCount+2));
                faces.push_back(Vector3<int>(vCount,vCount + 2,  vCount+1));

                faces.push_back(Vector3<int>(vCount+4, vCount + 7, vCount+6));
                faces.push_back(Vector3<int>(vCount+4, vCount +6, vCount+5));
                dashed = true;
            } else {
                faces.push_back(Vector3<int>(vCount, vCount + 2, vCount+3));
                faces.push_back(Vector3<int>(vCount,vCount + 1,  vCount+2));

                faces.push_back(Vector3<int>(vCount+4, vCount + 6, vCount+7));
                faces.push_back(Vector3<int>(vCount+4, vCount + 5, vCount+6));
                vCount += 8;
                dashed = false;
            }
        }
    }

    ThreeDObject returned;
    int verticeSize = vertices.size();
    int faceSize = faces.size();
    Vector3<float>* vertice = new Vector3<float>[verticeSize];
    Vector3<int>* face = new Vector3<int>[faceSize];
    std::copy(vertices.begin(), vertices.end(), vertice);
    std::copy(faces.begin(), faces.end(), face);
    returned.vertices = vertice;
    returned.faces = face;
    returned.vertice_count = verticeSize;
    returned.face_count = faceSize;
    return returned;
}

ThreeDObject GenerateParts(const CanvasPoints points[], const size_t start, const size_t end, const StreetConfig Gconfig, bool sided, float slopeDis){
    vector<Vector3<float>> vertices;
    vector<Vector3<int>> faces;
    // fix false sharing
    StreetConfig config = Gconfig;
    int vCount = 0;
    int vCountPre = 0;
    int vCountTotal = 0;
    float laneend = config.centerWidth + config.laneCount * config.laneWidth;
    float laneDivideEnd = laneend+config.laneDivderWidth;
    float laneShoulderEnd;
    if (config.laneDivide) {
        laneShoulderEnd = laneDivideEnd + config.shoulderWidth;
    } else {
        laneShoulderEnd = laneend + config.shoulderWidth;
    }
    for (size_t i = start; i < end; i++) {
        Vector2<float> fixedTangent = points[i].tan;
        fixedTangent *= (pow(-1, sided));
        Vector2<float> currentPosition = points[i].location;
        float currentDistance = 0;
        float paveDistance = 0;
        
        while (paveDistance < 5) {
            // most inner case 
            if(config.inner && currentDistance < config.innerWidth) {
                
                // edge condition
                if (currentDistance + DEFINITIONSTEP >= config.innerWidth) {
                    float tempDistance = config.innerWidth - currentDistance;
                    currentPosition += fixedTangent * tempDistance;
                    currentDistance += tempDistance;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.innerHeight, -currentPosition.y));
                    // upper slope
                    
                    currentDistance += slopeDis;
                    currentPosition += fixedTangent * slopeDis;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.centerHeight, -currentPosition.y));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                } else {
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                    config.innerHeight, -currentPosition.y ));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                }               
            } 

            // Central top
            else if(currentDistance < config.centerWidth) {
                // edge condition
                if (currentDistance + DEFINITIONSTEP >= config.centerWidth) {
                    float tempDistance = config.centerWidth - currentDistance;
                    currentPosition += fixedTangent * tempDistance;
                    currentDistance += tempDistance;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.centerHeight, -currentPosition.y));
                    // upper slope
                    currentDistance += slopeDis;
                    currentPosition += fixedTangent * slopeDis;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        0.0, -currentPosition.y));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                } else {
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                    config.centerHeight, -currentPosition.y ));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                }
                // next step
            }

            else if(currentDistance < laneend) {
                // edge condition
                if (currentDistance + DEFINITIONSTEP >= laneend) {
                    float tempDistance = laneend - currentDistance;
                    currentPosition += fixedTangent * tempDistance;
                    currentDistance += tempDistance;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        0.0, -currentPosition.y));
                    // upper slope
                    if (config.laneDivide){
                        currentDistance += slopeDis;
                        currentPosition += fixedTangent * slopeDis;
                        vertices.push_back(Vector3<float>(currentPosition.x, 
                            config.laneDiverHeight, -currentPosition.y));
                    }
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                } else {
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        0.0, -currentPosition.y ));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                }
                // next step
            }

            else if(config.laneDivide && currentDistance < laneDivideEnd) {
                // edge condition
                if (currentDistance + DEFINITIONSTEP >= laneDivideEnd) {
                    float tempDistance = laneDivideEnd - currentDistance;
                    currentPosition += fixedTangent * tempDistance;
                    currentDistance += tempDistance;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.laneDiverHeight, -currentPosition.y));
                    // upper slope
                    currentDistance += slopeDis;
                    currentPosition += fixedTangent * slopeDis;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        0.0, -currentPosition.y));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 

                } else {
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.laneDiverHeight, -currentPosition.y ));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                }
                // next step
            }
            else if(currentDistance < laneShoulderEnd) {
                // edge condition
                if (currentDistance + DEFINITIONSTEP >= laneShoulderEnd) {
                    float tempDistance = laneShoulderEnd - currentDistance;
                    currentPosition += fixedTangent * tempDistance;
                    currentDistance += tempDistance;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        0.0, -currentPosition.y));
                    // upper slope
                    currentDistance += slopeDis;
                    currentPosition += fixedTangent * slopeDis;
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.paveHeight, -currentPosition.y));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 

                } else {
                    vertices.push_back(Vector3<float>(currentPosition.x, 
                        0.0, -currentPosition.y ));
                    currentPosition += fixedTangent * DEFINITIONSTEP;
                    currentDistance += DEFINITIONSTEP; 
                }
                // next step
            }
            else {
                vertices.push_back(Vector3<float>(currentPosition.x, 
                        config.paveHeight, -currentPosition.y ));
                currentPosition += fixedTangent * DEFINITIONSTEP;
                currentDistance += DEFINITIONSTEP; 
                paveDistance += DEFINITIONSTEP;
            }
        }

        // connecting the vertices using faces
        if (i != start) {
            vCountPre = vCount;
            vCount = vertices.size() - vCountTotal;
            int vCountPrePointer = vCountTotal - vCountPre;
            int vCountPointer = vCountTotal;
            vCountTotal = vertices.size();
            for (int j = 0; j < vCount - 1; j++) {
                faces.push_back(Vector3<int>(vCountPrePointer + j, vCountPointer + j, vCountPointer+1 + j));
                faces.push_back(Vector3<int>(vCountPrePointer + j,  vCountPointer+1 + j, vCountPrePointer + 1 + j));
            }
            //  cout<<"layer "<<i<<" Vcount"<<vCount<<"\n";
        } else {
            vCount = vertices.size();
            vCountTotal = vCount;
            // cout<<"layer "<<i<<"Vcount"<<vCount<<"\n";
        }
        

    }
    ThreeDObject returned;
    int verticeSize = vertices.size();
    int faceSize = faces.size();
    Vector3<float>* vertice = new Vector3<float>[verticeSize];
    Vector3<int>* face = new Vector3<int>[faceSize];
    std::copy(vertices.begin(), vertices.end(), vertice);
    std::copy(faces.begin(), faces.end(), face);
    returned.vertices = vertice;
    returned.faces = face;
    returned.vertice_count = verticeSize;
    returned.face_count = faceSize;
    return returned;
}
