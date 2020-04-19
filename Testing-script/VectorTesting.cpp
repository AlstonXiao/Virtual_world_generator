#include "DataType.h"
#include <iostream>
#include <cstdlib>
using namespace std;
void
test(){
    Vector3<float> a(1.0, 2.0, 3.0);
    Vector3<float> b(3.0, 4.0, 5.0);
    Vector3<int> c(10, 20, 30);
    Vector3<int> d(30, 40, 50);
    a += 3.0f;
    cout<<a;
    a += b;
    cout<<a;
    Vector3<int> e = c+d;
    cout<<e;
    e.x = 0;
    cout<<e;
    cout<<c;
    cout<<d;
    d *= 3;
    cout<<d;
    cout<<"\n";
}

int main(){
    Vector2<float> a(1.0, 2.0);
    Vector2<float> b(3.0, 4.0);
    Vector2<int> c(10, 20);
    Vector2<int> d(30, 40);
    a += 3.0f;
    cout<<a;
    a += b;
    cout<<a;
    Vector2<int> e = c+d;
    cout<<e;
    e.x = 0;
    cout<<e;
    cout<<c;
    cout<<d;
    d *= 3;
    cout<<d;
    cout<<"\n";
    test();
    
}

