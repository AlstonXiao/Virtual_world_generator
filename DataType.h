#include <cstddef>

typedef struct vector2 {
    float x, y;
    vector2(float _x, float _y ): x(_x), y(_y) {};
} Vector2;

typedef struct vector3 {
    float x, y, z;
    vector2(float _x, float _y, float _z ): x(_x), y(_y), z(_z) {};
} Vector3;
