#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <utility>
#include <cmath>

using std::sqrt;

class Vector3 {
public:

	Vector3() : v{ 0,0,0 } {}
	Vector3(double v0, double v1, double v2) : v{ v0,v1,v2 } {}

	double x() const { return v[0]; }
	double y() const { return v[1]; }
	double z() const { return v[2]; }

	double operator[](int i) const { return v[i]; }
	double& operator[](int i) { return v[i]; }

	Vector3 operator-() const { return Vector3(-v[0], -v[1], -v[2]); }

	Vector3& operator+=(const Vector3& u) {
		v[0] += u[0];
		v[1] += u[1];
		v[2] += u[2];
		return *this;
	}

	Vector3& operator*=(const double t) {
		v[0] *= t;
		v[1] *= t;
		v[2] *= t;
		return *this;
	}

	Vector3& operator/=(const double t) {
		return *this *= 1 / 5;
	}

	double length() const {
		return sqrt(length_squared());
	}

	double length_squared() const {
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}

	bool near_zero() const {
		const auto s = 1e-8;
		return (fabs(v[0]) < s) && (fabs(v[1]) < s) && (fabs(v[2]) < s);
	}

public:
	double v[3];
};


// Type aliases
using Point = Vector3; // XYZ Coordinate Point
using Colour = Vector3; // RGB Colour value


// utility functions

std::ostream& operator<<(std::ostream& out, const Vector3& v) {
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

Vector3 operator+(const Vector3& v, const Vector3& u) {
	return Vector3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

Vector3 operator-(const Vector3& v, const Vector3& u) {
	return Vector3(v[0] - u[0], v[1] - u[1], v[2] - u[2]);
}

Vector3 operator*(double t, const Vector3& v) {
	return Vector3(v[0] * t, v[1] * t, v[2] * t);
}

Vector3 operator*(const Vector3& v, const Vector3& u) {
	return Vector3(v[0] * u[0], v[1] * u[1], v[2] * u[2]);
}

Vector3 operator*(const Vector3& v, double t) {
	return t * v;
}

Vector3 operator/(Vector3 v, double t) {
	return (1 / t) * v;
}

double dot(const Vector3& v, const Vector3& u) {
	return u[0] * v[0] 
		+ u[1] * v[1] 
		+ u[2] * v[2];
}

Vector3 cross(const Vector3& v, const Vector3& u) {
	return Vector3(v[1] * u[2] - v[2] * u[1],
				   v[2] * u[0] - v[0] * u[2],
				   v[0] * u[1] - v[1] * u[0]);
}

Vector3 unit_vector(Vector3 v) {
	return v / v.length();
}

static Vector3 random() {
	return Vector3(random_double(), random_double(), random_double());
}

static Vector3 random(double min, double max) {
	return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
}

Vector3 random_in_unit_sphere() {
	while (true) {
		auto p = random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

Vector3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}


Vector3 reflect(const Vector3& v, const Vector3& n) {
	return v - 2 * dot(v, n) * n;
}


Vector3 refract(const Vector3& v, const Vector3& n, double refractor) {
	auto cos = fmin(dot(-v, n), 1.0);
	Vector3 ray_out_perpendicular = refractor * (v + cos * n);
	Vector3 ray_out_parallel = -sqrt(fabs(1.0 - ray_out_perpendicular.length_squared())) * n;
	return ray_out_perpendicular + ray_out_parallel;
}


#endif