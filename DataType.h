#include <cstddef>
#include <array>
#include <string>
#include <iostream>
#include <math.h>

template<typename T>
struct Vector2 {
    T x, y;
    Vector2(T _x, T _y ): x(_x), y(_y) {};
    Vector2() :x(0), y(0) {};
    struct Vector2& operator+=(const Vector2<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
    struct Vector2& operator+=(const T& k) { x += k; y += k; return *this; }
    struct Vector2& operator-=(const Vector2<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    struct Vector2& operator-=(const T& k) { x -= k; y -= k; return *this; }
    struct Vector2& operator*=(const T& k) { x *= k; y *= k; return *this; }

    Vector2 operator+(const Vector2& rhs) {return Vector2(x + rhs.x, y + rhs.y);}
    Vector2 operator-(const Vector2& rhs) {return Vector2(x - rhs.x, y - rhs.y); }
    Vector2 operator+(const T k) {return Vector2(x + k, y + k); }
    Vector2 operator-(const T k) { return Vector2(x - k, y - k);}
    Vector2 operator*(const T k) {return Vector2(x * k, y * k); }
    Vector2 operator/(const T k) { return Vector2(x / k, y / k);}

    bool operator==(const Vector2& rhs) {return (x == rhs.x && y == rhs.y); }
    // Vector2 operator=(const Vector2& rhs) {return Vector2(rhs.x, rhs.y);}

    friend std::ostream & operator<<(std::ostream & out, const Vector2& rhs) {
        out << "(" << rhs.x << ", " << rhs.y << ")";
        return out;
    }
};

template<typename T>
struct Vector3 {
    T x, y, z;
    Vector3(T _x, T _y, T _z ): x(_x), y(_y), z(_z) {};
    Vector3() :x(0), y(0), z(0) {};
    struct Vector3& operator+=(const Vector3<T>& rhs) { x += rhs.x; y += rhs.y; z+=rhs.z; return *this; }
    struct Vector3& operator+=(const T& k) { x += k; y += k; z += k; return *this; }
    struct Vector3& operator-=(const Vector3<T>& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    struct Vector3& operator-=(const T& k) { x -= k; y -= k; z -= k; return *this; }
    struct Vector3& operator*=(const T& k) { x *= k; y *= k; z *= k; return *this; }

    Vector3 operator+(const Vector3& rhs) {return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);}
    Vector3 operator-(const Vector3& rhs) {return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3 operator+(const T k) {return Vector3(x + k, y + k, z + k); }
    Vector3 operator-(const T k) { return Vector3(x - k, y - k, z - k);}
    Vector3 operator*(const T k) {return Vector3(x * k, y * k, z * k); }
    Vector3 operator/(const T k) { return Vector3(x / k, y / k, z / k);}

    bool operator==(const Vector3& rhs) {return (x == rhs.x && y == rhs.y && z == rhs.z); }

    friend std::ostream & operator<<(std::ostream & out, const Vector3& rhs) {
        out << "(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";
        return out;
    }
};

template<typename T>
float distance(Vector3<T> p1, Vector3<T> p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z));
}

template<typename T>
float distance(Vector2<T> p1, Vector2<T> p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

typedef struct CanvasPoints{
    Vector2<float> location;
    Vector2<float> tan;
    float distance;
}CanvasPoints;

typedef struct ThreeDObject{
    Vector3<float>* vertices;
    Vector3<int>* faces;
    size_t vertice_count;
    size_t face_count;
}ThreeDObject;

typedef struct OutputObject {
    ThreeDObject* objects;
    size_t streetCount;
    size_t otherObjectCount;
}OutputObject;

class TwoD_function{
    public:
    Vector2<float> zeroDegree;
    Vector2<float> oneDegree;
    Vector2<float> twoDegree;
    Vector2<float> threeDegree;

    Vector2<float> zeroDegreeDri;
    Vector2<float> oneDegreeDri;
    Vector2<float> twoDegreeDri;
    TwoD_function(Vector2<float> _zeroDegree, Vector2<float> _oneDegree, Vector2<float> _twoDegree, Vector2<float> _threeDegree) {
        zeroDegree = _zeroDegree;
        oneDegree = _oneDegree;
        twoDegree = _twoDegree;
        threeDegree = _threeDegree;

        zeroDegreeDri = _oneDegree;
        oneDegreeDri = _twoDegree * 2;
        twoDegreeDri = _threeDegree * 3;
        //std::cout<<_zeroDegree<<oneDegree<<twoDegree<<threeDegree<<zeroDegreeDri<<oneDegreeDri<<twoDegreeDri<<"\n";
    }

    Vector2<float> at(float u) {
        return (zeroDegree + oneDegree*u + twoDegree*u*u + threeDegree*u*u*u);
    }

    Vector2<float> dirvAt(float u) {
        return (zeroDegreeDri + oneDegreeDri*u + twoDegreeDri*u*u);
    }
};

typedef struct StreetConfig{
    float centerWidth;
    float centerHeight;
    bool inner;
    float innerWidth;
    float innerHeight;

    int laneCount;
    float laneWidth;

    bool laneDivide;
    float laneDiverHeight;
    float laneDivderWidth;

    float shoulderWidth;
    float paveHeight;
    
    friend std::ostream & operator<<(std::ostream & out, const StreetConfig& rhs) {
        out << "center Width: " << rhs.centerWidth << ", center Height: " << rhs.centerHeight<<"\n";
        if (rhs.inner) {
            out << "inner: true, inner Width: " << rhs.innerWidth<<", inner Height: " << rhs.innerHeight<<"\n";
        } else {
            out << "inner: false\n";
        }
        out << "lane Count: " << rhs.laneCount << ", lane Width: " << rhs.laneWidth<<"\n";
        if (rhs.laneDivide) {
            out << "lane Divide: true, lane Diver Height: " << rhs.laneDiverHeight<<"\n";
        } else {
            out << "lane Divide: false\n";
        }
        
        out << "shoulder Width: " << rhs.shoulderWidth<<"\n";        
        out << "pave Height: " << rhs.paveHeight<<"\n";
        return out;
    }
}StreetConfig;

typedef struct StreetConfigExtra{
    bool extraSpace;
    float extraSpaceWidth;
    float extraSpaceDistance;

    friend std::ostream & operator<<(std::ostream & out, const StreetConfigExtra& rhs) {
        if (rhs.extraSpace){
            out << "extra Space: true, extra SpaceWidth: " << rhs.extraSpaceWidth<<", extra Space Distance: " << rhs.extraSpaceDistance<<"\n";
        } else {
            out << "extra Space: false\n";
        }
        
        return out;
    }
}StreetConfigExtra;