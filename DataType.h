#include <cstddef>
#include <array>
#include <string>

template<typename T>
struct Vector2 {
    T x, y;
    Vector2(T _x, T _y ): x(_x), y(_y) {};
    struct Vector2& operator+=(const Vector2<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
    struct Vector2& operator+=(const T& k) { x += k; y += k; return *this; }
    struct Vector2& operator-=(const Vector2<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    struct Vector2& operator-=(const T& k) { x -= k; y -= k; return *this; }

    Vector2 operator+(const Vector2& rhs) {return Vector2(x + rhs.x, y + rhs.y);}
    Vector2 operator-(const Vector2& rhs) {return Vector2(x - rhs.x, y - rhs.y); }
    Vector2 operator+(const T k) {return Vector2(x + k, y + k); }
    Vector2 operator-(const T k) { return Vector2(x - k, y - k);}

    bool operator=(const Vector2& rhs) {return if(x == rhs.x && y == lhs.y); }

    friend std::ostream & operator<<(std::ostream & out, const Vector2& rhs) {
        out<<"("<<rhs.x<<", "<<rhs.y<<")";
        return out;
    }
};



// typedef struct Vector3 {
//     float x, y, z;
//     Vector3(float _x, float _y, float _z ): x(_x), y(_y), z(_z) {};
//     struct Vector3& operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
//     struct Vector3& operator+=(const float& k) { x += k; y += k; z += k; return *this; }
//     struct Vector3& operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
//     struct Vector3& operator-=(const float& k) { x -= k; y -= k; z -= k; return *this; }
// } Vector3;

// Vector3 operator+(Vector3 lhs, const Vector3& rhs) { return lhs += rhs; }
// Vector3 operator-(Vector3 lhs, const Vector3& rhs) { return lhs -= rhs; }
// Vector3 operator+(Vector3 lhs, const float k) { return lhs += k; }
// Vector3 operator-(Vector3 lhs, const float k) { return lhs -= k; }
// Vector3 operator+(const float k, Vector3 rhs) { return rhs += k; }