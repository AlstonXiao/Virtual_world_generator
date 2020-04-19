#include <cstddef>

#ifndef DATATYPE_H
#define DATATYPE_H
#include "DataType.h"
#endif


#define PARAPOINTS 10

// This function will process the controll points of the bezier curve and
// turn them into arc-length parametrized points and tangent
CanvasPoints* ProcessPoints(Vector2<float>* controllPoints);
