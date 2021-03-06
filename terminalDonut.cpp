#include <iostream>
#include <cmath>
#include <thread> // just for this_thread::sleep_for

struct Vector3 {
    float x = 0.f, y = 0.f, z = 0.f;

    Vector3() {}
    Vector3( float x_val, float y_val, float z_val ) : x( x_val ), y( y_val ), z( z_val ) {}
    Vector3( const float arr[3] ) : x( arr[0] ), y( arr[1] ), z( arr[2] ) {}

    static Vector3 in_xy_circle( float radians ) { return Vector3{ std::cos( radians ), std::sin( radians ), 0.f }; }

    Vector3 operator+( const Vector3 &other ) const { return Vector3{ x + other.x, y + other.y, z + other.z }; }
    Vector3 operator-( const Vector3 &other ) const { return Vector3{ x - other.x, y - other.y, z - other.z }; }
    Vector3 operator*( const float &factor ) const { return Vector3{ x * factor, y * factor, z * factor }; }

    float dot_product( const Vector3 &other ) const { return x * other.x + y * other.y + z * other.z; }
    float length() const { return std::sqrt( this->dot_product( *this ) ); }
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
    // Create a simple ring in the xy-plane
    const size_t RING_SECTIONS = 64;
    Vector3 basic_ring[RING_SECTIONS];
    for ( size_t i = 0; i < RING_SECTIONS; i++ )
        basic_ring[i] = Vector3::in_xy_circle( i * M_PI * 2 / RING_SECTIONS );

    // Create the torus
    const size_t TORUS_SECTIONS = 256;
    Vector3 torus[RING_SECTIONS * TORUS_SECTIONS];
    Vector3 normals[RING_SECTIONS * TORUS_SECTIONS];
    Vector3 index = Vector3{ 2.f, 0.f, 0.f };
    for ( size_t i = 0; i < TORUS_SECTIONS; i++ ) {
        auto rotation = Matrix3::rotationAroundY( i * M_PI * 2 / TORUS_SECTIONS );
        for ( size_t j = 0; j < RING_SECTIONS; j++ ) {
            normals[i * RING_SECTIONS + j] = rotation * basic_ring[j];
            torus[i * RING_SECTIONS + j] = rotation * ( index + basic_ring[j] );
        }
    }

    // Some data
    bool running = true;
    float character_ratio = 9.f / 20.f;
    const size_t BUFFER_SIZE_X = 64, BUFFER_SIZE_Y = BUFFER_SIZE_X * character_ratio;
    float render_buffer[BUFFER_SIZE_X][BUFFER_SIZE_Y];
    float z_buffer[BUFFER_SIZE_X][BUFFER_SIZE_Y];
    Vector3 center = Vector3{ 32.f, 32.f, 32.f };
    Vector3 light_dir = Vector3{ -0.4f, -1.f, 0.4f };
    light_dir = light_dir * ( 1.f / light_dir.length() );
    float time = 0.f;

    // Main loop
    while ( running ) {
        auto x_rotation = Matrix3::rotationAroundX( M_PI / 2.f * std::sin( time * 1.5 ) );
        auto y_rotation = Matrix3::rotationAroundY( M_PI / 2.f * std::sin( time * 1.5 + 1.5 ) );
        auto z_rotation = Matrix3::rotationAroundZ( M_PI / 2.f * std::sin( time + 1.5 ) );
        // Clear buffer
        for ( size_t y = 0; y < BUFFER_SIZE_Y; y++ ) {
            for ( size_t x = 0; x < BUFFER_SIZE_X; x++ ) {
                render_buffer[x][y] = 0.f;
                z_buffer[x][y] = 0.f;
            }
        }

        // Rotate and project the torus
        for ( size_t i = 0; i < TORUS_SECTIONS * RING_SECTIONS; i++ ) {
            auto final_position = z_rotation * ( y_rotation * ( x_rotation * torus[i] ) );
            auto final_normal = z_rotation * ( y_rotation * ( x_rotation * normals[i] ) );
            final_position = final_position * 8.f + center; // adjust position into viewport
            int x = static_cast<int>( std::round( final_position.x ) );
            int y = static_cast<int>( std::round( final_position.y * character_ratio ) );
            int z = static_cast<int>( std::round( final_position.z ) );
            if ( z_buffer[x][y] + 0.1f < z ) {
                render_buffer[x][y] = final_normal.dot_product( light_dir );
                z_buffer[x][y] = final_position.z;
            } else if ( std::abs( z_buffer[x][y] - z ) < 0.1f ) {
                render_buffer[x][y] = ( render_buffer[x][y] + final_normal.dot_product( light_dir ) ) / 2.f;
            }
        }

        // Render to terminal
        std::cout << "\033[3J\033[1;1H";
        for ( size_t y = 0; y < BUFFER_SIZE_Y; y++ ) {
            for ( size_t x = 0; x < BUFFER_SIZE_X; x++ ) {
                if ( render_buffer[x][y] == 0.f ) {
                    std::cout << ' ';
                } else {
                    int val = render_buffer[x][y] * 6 + 6;
                    std::cout << ( (char *) ".,-~:;=!*#$@" )[val];
                }
            }
            std::cout << "\n";
        }
        std::cout << std::flush;

        // Ensure ~60 FPS
        std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) );
        time += 0.015;
    }

    return 0;
}
