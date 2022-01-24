
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

double degrees_to_radians(double deg) {
	return deg * pi / 180;
}

double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

int random_int(int min, int max){
	return static_cast<int>(random_double(min,max+1));
}

double clamp(double x, double min, double max) {
	if (x < min)return min;
	if (x > max)return max;
	return x;
}

double deg2rad(double degrees){
	return degrees*(pi/180);
}

#include "ray.h"
#include "vector3.h"

#endif
