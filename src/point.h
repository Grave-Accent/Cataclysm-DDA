#pragma once
#ifndef CATA_POINT_H
#define CATA_POINT_H

#include <array>
#include <cassert>
#include <climits>
#include <functional>
#include <ostream>

class JsonOut;
class JsonIn;

struct point {
    int x = 0;
    int y = 0;
    constexpr point() = default;
    constexpr point( int X, int Y ) : x( X ), y( Y ) {}

    constexpr point operator+( const point &rhs ) const {
        return point( x + rhs.x, y + rhs.y );
    }
    point &operator+=( const point &rhs ) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    constexpr point operator-() const {
        return point( -x, -y );
    }
    constexpr point operator-( const point &rhs ) const {
        return point( x - rhs.x, y - rhs.y );
    }
    point &operator-=( const point &rhs ) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    constexpr point operator*( const int rhs ) const {
        return point( x * rhs, y * rhs );
    }
    point &operator*=( const int rhs ) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    constexpr point operator/( const int rhs ) const {
        return point( x / rhs, y / rhs );
    }

    /**
     * Rotate point clockwise @param turns times, 90 degrees per turn,
     * around the center of a rectangle with the dimensions specified
     * by @param dim. By default rotates around the origin (0, 0).
     */
    point rotate( int turns, const point &dim = { 1, 1 } ) const {
        assert( turns >= 0 );
        assert( turns <= 4 );

        switch( turns ) {
            case 1:
                return { dim.y - y - 1, x };
            case 2:
                return { dim.x - x - 1, dim.y - y - 1 };
            case 3:
                return { y, dim.x - x - 1 };
        }

        return *this;
    }

    std::string to_string() const;
};

std::ostream &operator<<( std::ostream &, const point & );

void serialize( const point &p, JsonOut &jsout );
void deserialize( point &p, JsonIn &jsin );

// Make point hashable so it can be used as an unordered_set or unordered_map key,
// or a component of one.
namespace std
{
template <>
struct hash<point> {
    std::size_t operator()( const point &k ) const {
        constexpr uint64_t a = 2862933555777941757;
        size_t result = k.y;
        result *= a;
        result += k.x;
        return result;
    }
};
} // namespace std

inline constexpr bool operator<( const point &a, const point &b )
{
    return a.x < b.x || ( a.x == b.x && a.y < b.y );
}
inline constexpr bool operator==( const point &a, const point &b )
{
    return a.x == b.x && a.y == b.y;
}
inline constexpr bool operator!=( const point &a, const point &b )
{
    return !( a == b );
}

inline point abs( const point &p )
{
    return point( abs( p.x ), abs( p.y ) );
}

struct tripoint {
    int x = 0;
    int y = 0;
    int z = 0;
    constexpr tripoint() = default;
    constexpr tripoint( int X, int Y, int Z ) : x( X ), y( Y ), z( Z ) {}
    explicit constexpr tripoint( const point &p, int Z ) : x( p.x ), y( p.y ), z( Z ) {}

    constexpr tripoint operator+( const tripoint &rhs ) const {
        return tripoint( x + rhs.x, y + rhs.y, z + rhs.z );
    }
    constexpr tripoint operator-( const tripoint &rhs ) const {
        return tripoint( x - rhs.x, y - rhs.y, z - rhs.z );
    }
    tripoint &operator+=( const tripoint &rhs ) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    constexpr tripoint operator-() const {
        return tripoint( -x, -y, -z );
    }
    constexpr tripoint operator*( const int rhs ) const {
        return tripoint( x * rhs, y * rhs, z * rhs );
    }
    tripoint &operator*=( const int rhs ) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    /*** some point operators and functions ***/
    constexpr tripoint operator+( const point &rhs ) const {
        return tripoint( x + rhs.x, y + rhs.y, z );
    }
    constexpr tripoint operator-( const point &rhs ) const {
        return tripoint( x - rhs.x, y - rhs.y, z );
    }
    tripoint &operator+=( const point &rhs ) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    tripoint &operator-=( const point &rhs ) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    tripoint &operator-=( const tripoint &rhs ) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    point xy() const {
        return point( x, y );
    }

    std::string to_string() const;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

std::ostream &operator<<( std::ostream &, const tripoint & );

// Make tripoint hashable so it can be used as an unordered_set or unordered_map key,
// or a component of one.
namespace std
{
template <>
struct hash<tripoint> {
    std::size_t operator()( const tripoint &k ) const {
        constexpr uint64_t a = 2862933555777941757;
        size_t result = k.z;
        result *= a;
        result += k.y;
        result *= a;
        result += k.x;
        return result;
    }
};
} // namespace std

inline constexpr bool operator==( const tripoint &a, const tripoint &b )
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}
inline constexpr bool operator!=( const tripoint &a, const tripoint &b )
{
    return !( a == b );
}
inline bool operator<( const tripoint &a, const tripoint &b )
{
    if( a.x != b.x ) {
        return a.x < b.x;
    }
    if( a.y != b.y ) {
        return a.y < b.y;
    }
    if( a.z != b.z ) {
        return a.z < b.z;
    }
    return false;
}

static const std::array<tripoint, 8> eight_horizontal_neighbors = { {
        { -1, -1, 0 },
        {  0, -1, 0 },
        { +1, -1, 0 },
        { -1,  0, 0 },
        { +1,  0, 0 },
        { -1, +1, 0 },
        {  0, +1, 0 },
        { +1, +1, 0 },
    }
};

struct rectangle {
    point p_min;
    point p_max;
    constexpr rectangle() = default;
    constexpr rectangle( const point &P_MIN, const point &P_MAX ) : p_min( P_MIN ), p_max( P_MAX ) {}

    constexpr bool contains_half_open( const point &p ) const {
        return p.x >= p_min.x && p.x < p_max.x &&
               p.y >= p_min.y && p.y < p_max.y;
    }

    constexpr bool contains_inclusive( const point &p ) const {
        return p.x >= p_min.x && p.x <= p_max.x &&
               p.y >= p_min.y && p.y <= p_max.y;
    }
};

// Clamp p to the half-open rectangle r.
// This independently clamps each coordinate of p to the bounds of the
// rectangle.
// Useful for example to round an arbitrary point to the nearest point on the
// screen, or the nearest point in a particular submap.
point clamp_half_open( const point &p, const rectangle &r );

struct box {
    tripoint p_min;
    tripoint p_max;
    constexpr box() = default;
    constexpr box( const tripoint &P_MIN, const tripoint &P_MAX ) : p_min( P_MIN ), p_max( P_MAX ) {}
    explicit constexpr box( const rectangle &R, int Z1, int Z2 ) :
        p_min( tripoint( R.p_min, Z1 ) ), p_max( tripoint( R.p_max, Z2 ) ) {}

    constexpr bool contains_half_open( const tripoint &p ) const {
        return p.x >= p_min.x && p.x < p_max.x &&
               p.y >= p_min.y && p.y < p_max.y &&
               p.z >= p_min.z && p.z < p_max.z;
    }

    constexpr bool contains_inclusive( const tripoint &p ) const {
        return p.x >= p_min.x && p.x <= p_max.x &&
               p.y >= p_min.y && p.y <= p_max.y &&
               p.z >= p_min.z && p.z <= p_max.z;
    }

    void shrink( const tripoint &amount ) {
        p_min += amount;
        p_max -= amount;
    }
};

static constexpr tripoint tripoint_min { INT_MIN, INT_MIN, INT_MIN };
static constexpr tripoint tripoint_zero { 0, 0, 0 };
static constexpr tripoint tripoint_max{ INT_MAX, INT_MAX, INT_MAX };

static constexpr point point_min{ tripoint_min.x, tripoint_min.y };
static constexpr point point_zero{ tripoint_zero.x, tripoint_zero.y };
static constexpr point point_max{ tripoint_max.x, tripoint_max.y };

static constexpr point point_north{ 0, -1 };
static constexpr point point_north_east{ 1, -1 };
static constexpr point point_east{ 1, 0 };
static constexpr point point_south_east{ 1, 1 };
static constexpr point point_south{ 0, 1 };
static constexpr point point_south_west{ -1, 1 };
static constexpr point point_west{ -1, 0 };
static constexpr point point_north_west{ -1, -1 };

static constexpr std::array<point, 4> four_adjacent_offsets{{
        point_north, point_east, point_south, point_west
    }};

static constexpr tripoint tripoint_above{ 0, 0, 1 };
static constexpr tripoint tripoint_below{ 0, 0, -1 };

static constexpr box box_zero( tripoint_zero, tripoint_zero );
static constexpr rectangle rectangle_zero( point_zero, point_zero );

struct sphere {
    int radius = 0;
    tripoint center = tripoint_zero;

    sphere() = default;
    explicit sphere( const tripoint &center ) : radius( 1 ), center( center ) {}
    explicit sphere( const tripoint &center, int radius ) : radius( radius ), center( center ) {}
};

#endif // CATA_POINT_H
