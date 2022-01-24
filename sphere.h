

#ifndef SPHERE_H
#define SPHERE_H


#include "object.h"
#include "vector3.h"

class Sphere : public Object {
public:
	Sphere() {}
	Sphere(Point c, double r, shared_ptr<Material> m) : center(c), radius(r), material(m){};

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	static void get_UV(const Point& p, double& u, double& v){
		auto angle1 = acos(-p.y());
		auto angle2 = atan2(-p.z(), p.x()) + pi;

		u = angle2 / 2*pi;
		v = angle1 / pi;
	}

public:
	Point center;
	double radius;
	shared_ptr<Material> material;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	Vector3 OC = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(OC, r.direction());
	auto c = OC.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root - (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	Vector3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.material_ptr = material;

	get_UV(outward_normal, rec.u, rec.v);

	return true;
}

#endif