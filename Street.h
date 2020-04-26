#include <omp.h>

ThreeDObject* generateStreet(CanvasPoints points[]);
ThreeDObject GenerateParts(const CanvasPoints points[], const size_t start, const size_t end, const StreetConfig config, bool sided);