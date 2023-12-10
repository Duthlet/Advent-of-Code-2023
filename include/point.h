#pragma once

#include <shortinttypes.h>

struct point {
    i32 x;
    i32 y;

    friend auto operator<=>(const point&, const point&) = default;

    point& operator+=(const point& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    point& operator-=(const point& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    point& operator*=(const i32& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

	point up() const { return point{x, y-1}; }
	point down() const { return point{x, y+1}; }
	point right() const { return point{x+1, y}; }
	point left() const { return point{x-1, y}; }
	explicit operator u64() const {
		return (u64(u32(x)) << 32) & u64(u32(y));
	}
};

template <>
struct std::hash<point>
{
    std::size_t operator()(const point& p) const
    {
        return (std::hash<u64>()(u64(p)));
    }
};

inline point operator+(const point& lhs, const point& rhs)
{
    point result = lhs;
    result += rhs;
    return result;
}

inline point operator-(const point& lhs, const point& rhs)
{
    point result = lhs;
    result -= rhs;
    return result;
}

inline point operator*(const point& pt, const i32& scalar)
{
    point result = pt;
    result *= scalar;
    return result;
}

inline point operator*(const i32& scalar, const point& pt)
{
    return pt*scalar;
}

std::ostream& operator<<(std::ostream& os, const point& p)
{
  return os << "(" << p.x << ", " << p.y << ")";
}

