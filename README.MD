# virtual World Generator
## Project detail
This project can generate a 3D world models based on user defined parameter and randomization. Each generation will output the world with detailed and unique streets, buildings, decorations. User can define some parameter like the direction of the street, number of trees, height of the trees. The program will generate 3D meshes from basic vertices and faces, with the information and randomization in some of the detailed. The entire generation process will be paralleled and finally merge them together to form the 3D world
## How you will demonstrate what you accomplished: 
This is a huge project and cannot be done within one month. I want to deliver a simplest version of the generator. It will include a curved street, trees, and maybe buildings. User can define the curve and maybe some other parameters. Then the program will produce the 3D model for the street view. Moreover, there will be no texture on the model.
## Current Stage
 * Preparing taking input from the user
 * Building a graphical interface for user to take input
 * Fragmenting the street for each OpenMP or CUDA process
 * What kind of randomization is needed 
## input output
### Input 
Should be 4 points on a 500 * 500 canvas, each points is a control points for the bezier curve
### Output
Should be a file
## Files
* main.cpp: take in 4 2D dots, then output to a obj file
* Bezier: Bezier curve processing.
## Testing
Testing 1: bezier curve from control points to dots
