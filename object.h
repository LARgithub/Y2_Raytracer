

#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#include "aabb.h"

class Material;

struct HitRecord {
	Point p;
	Vector3 normal;
	double t;
	shared_ptr<Material> material_ptr;
	bool front_face;

	double u,v;

	inline void set_face_normal(const Ray& r, const Vector3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Object {
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};


#endif