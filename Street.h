#include <omp.h>

OutputObject generateStreet(CanvasPoints points[]);
ThreeDObject GenerateParts(const CanvasPoints points[], const size_t start, const size_t end, const StreetConfig config, bool sided);
ThreeDObject GenerateSignDashed(const CanvasPoints points[], const float position, const float linewitdth);
ThreeDObject GenerateSign(const CanvasPoints points[], const float position, const float linewitdth);
ThreeDObject GenerateBars(const CanvasPoints points[], const float position);