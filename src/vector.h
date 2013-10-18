#pragma once

#include <cmath>
#include <cassert>

class Vector3 {
public:
	double x,y,z;

	Vector3() : x(0), y(0), z(0) {
	}

	Vector3(double x_, double y_, double z_) 
		: x(x_)
		, y(y_)
		, z(z_)
	{}

	Vector3(const Vector3 &r) : x(r.x), y(r.y), z(r.z) {
	}

	Vector3 &operator += (const Vector3 &r) {
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}
	Vector3 &operator -= (const Vector3 &r) {
		x -= r.x;
		y -= r.y;
		z -= r.z;
		return *this;
	}
	
	Vector3 &operator *= (double v) {
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
	Vector3 &operator /= (double v) {
		assert(v != 0);
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}
  Vector3 operator * (double v) const {
    return Vector3(x*v, y*v, z*v);
  }
  Vector3 operator / (double v) const {
    return Vector3(x/v, y/v, z/v);
  }
  Vector3 operator + (const Vector3 &v) const {
    return Vector3(x+v.x, y+v.y, z+v.z);
  }
  Vector3 operator - (const Vector3 &v) const {
    return Vector3(x-v.x, y-v.y, z-v.z);
  }
	double dot(const Vector3 &r) const {
		return x*r.x + y*r.y + z*r.z;
	}

	Vector3 cross(const Vector3 &r) const {
		return Vector3(
			y*r.z - z*r.y,
			z*r.x - x*r.z,
			x*r.y - y*r.x
		);
	}
	double length() const {
		return std::sqrt(lengthSq());
	}
	double lengthSq() const {
		return x*x + y*y + z*z;
	}
	void normalize() {
		(*this) /= length();
	}
};
