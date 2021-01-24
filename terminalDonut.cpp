#include <iostream>
#include <cmath>

struct Vector3 {
    float x = 0.f, y = 0.f, z = 0.f;

    Vector3( float x_val, float y_val, float z_val ) : x( x_val ), y( y_val ), z( z_val ) {}
    Vector3( const float arr[3] ) : x( arr[0] ), y( arr[1] ), z( arr[2] ) {}

    Vector3 operator+( const Vector3 &other ) const { return Vector3{ x + other.x, y + other.y, z + other.z }; }
    Vector3 operator-( const Vector3 &other ) const { return Vector3{ x - other.x, y - other.y, z - other.z }; }

    float dot_product( const Vector3 &other ) const { return x * other.x + y * other.y + z * other.z; }
};

struct Matrix3 {
    float val[3][3];

    static Matrix3 identity() { return Matrix3{ { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f } } }; }
    static Matrix3 rotationAroundX( float radians ) {
        return Matrix3{ { { 1.f, 0.f, 0.f },
                          { 0.f, std::cos( radians ), -std::sin( radians ) },
                          { 0.f, std::sin( radians ), std::cos( radians ) } } };
    }
    static Matrix3 rotationAroundY( float radians ) {
        return Matrix3{ { { std::cos( radians ), 0.f, std::sin( radians ) },
                          { 0.f, 1.f, 0.f },
                          { -std::sin( radians ), 0.f, std::cos( radians ) } } };
    }
    static Matrix3 rotationAroundZ( float radians ) {
        return Matrix3{ { { std::cos( radians ), -std::sin( radians ), 0.f },
                          { std::sin( radians ), std::cos( radians ), 0.f },
                          { 0.f, 0.f, 1.f } } };
    }

    Vector3 operator*( const Vector3 &vec ) const {
        return Vector3{ Vector3( val[0] ).dot_product( vec ), Vector3( val[1] ).dot_product( vec ),
                        Vector3( val[2] ).dot_product( vec ) };
    }
};

int main() {
    auto m = Matrix3::rotationAroundX( M_PI );
    std::cout << m.val[1][0] << " " << m.val[1][1] << " " << m.val[1][2] << std::endl;
    return 0;
}
