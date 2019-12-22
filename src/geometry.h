#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** 2d mathematical vector */
template <class t> struct Vec2 {
	union {
		// Texture coordinates names
		struct { t u, v; };
		// Any other coords except texture
		struct { t x, y; };
		// Just tuple of coordinates
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u), v(_v) {}
	// Vec + Vec
	inline Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(u + V.u, v + V.v); }
	inline Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(u - V.u, v - V.v); }

	// Vec + n
	inline Vec2<t> operator +(const t other) const { return Vec2<t>(u + other, v + other); }
	inline Vec2<t> operator *(const t other) const { return Vec2<t>(u * other, v * other); }

	// Vec += Vec
	inline Vec2<t>& operator +=(const Vec2<t>& v) { x += v.x; y += v.y; return *this; }
	inline Vec2<t>& operator -=(const Vec2<t>& v) { x -= v.x; y -= v.y; return *this; }

	// Vec += n
	inline Vec2<t>& operator +=(const t other) { x += other; y += other; return *this; }
	inline Vec2<t>& operator -=(const t other) { x -= other; y -= other; return *this; }

	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

/** 3d mathematical vector */
template <class t> struct Vec3 {
	union {
		struct { t x, y, z; };
		// For face
		struct { t ivert, iuv, inorm; };
		// Just tuple of coordinates
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
	// Vec + Vec
	inline Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	inline Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
	inline Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
	inline t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; } // Scalar mult

	// Vec += Vec
	inline Vec3<t>& operator +=(const Vec3<t>& v) { x += v.x; y += v.y, z += v.z; return *this; }
	inline Vec3<t>& operator -=(const Vec3<t>& v) { x -= v.x; y -= v.y, z -= v.z; return *this; }
	inline Vec3<t>& operator ^=(const Vec3<t>& v) { x = y * v.z - z * v.y; y = z * v.x - x * v.z; z = x * v.y - y * v.x; return *this; }

	// Vec + n
	inline Vec3<t> operator +(const t other) const { return Vec3<t>(x + other, y + other, z + other); }
	inline Vec3<t> operator -(const t other) const { return Vec3<t>(x - other, y - other, z - other); }
	inline Vec3<t> operator *(const t other) const { return Vec3<t>(x * other, y * other, z * other); }
	inline Vec3<t> operator /(const t other) const { return Vec3<t>(x / other, y / other, z / other); }

	// Vec += n
	inline Vec3<t>& operator +=(const t other) { x += other; y += other, z += other; return *this; }
	inline Vec3<t>& operator -=(const t other) { x -= other; y -= other, z -= other; return *this; }
	inline Vec3<t>& operator *=(const t other) { x *= other; y *= other, z *= other; return *this; }
	inline Vec3<t>& operator /=(const t other) { x /= other; y /= other, z /= other; return *this; }

	/** Returns vector length */
	float norm() const { return std::sqrt(x * x + y * y + z * z); }

	/** Set vector length to given value */
	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }

	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

/** Float 2d math vector */
typedef Vec2<float> Vec2f;
/** Int 2d math vector */
typedef Vec2<int>   Vec2i;
/** Float 3d math vector */
typedef Vec3<float> Vec3f;
/** Int 2d math vector */
typedef Vec3<int>   Vec3i;

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return s;
}

#endif //__GEOMETRY_H__
