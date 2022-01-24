#ifndef RAY_H
#define RAY_H

#include "vector3.h"


class Ray {
public:
	Ray() {}
	Ray(const Point& origin, const Vector3& direction) 
			: org(origin), dir(direction) {}

	Point origin() const { return org; }
	Vector3 direction() const { return dir; }

	Point at(double t) const {
		return org + t * dir;
	}

public:
	Point org;
	Vector3 dir;
};

#endif

