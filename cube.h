#ifndef CUBE_H
#define CUBE_H

#include "utility.h"
#include "object.h"
#include "scene.h"


class Cube : public Object {
public:
	Cube() {}
	Cube(const Point& min, const Point& max, shared_ptr<Material> m) 
				: cube_min(min), cube_max(max), material(m) {};

	virtual bool hit(const Ray& r, double t_min, 
			double t_max, HitRecord& rec) const override;

public:
	Point cube_min;
	Point cube_max;
	shared_ptr<Material> material;
};

bool Cube::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {

	for(int a=0; a<3; a++){
		auto t0 = fmin((cube_min[a] - r.origin()[a]) / r.direction()[a],
					   (cube_max[a] - r.origin()[a]) / r.direction()[a]);
		auto t1 = fmax((cube_min[a] - r.origin()[a]) / r.direction()[a],
					   (cube_max[a] - r.origin()[a]) / r.direction()[a]);
		
		t_min = fmax(t0, t_min);
		t_max = fmin(t1, t_max);

		if(t_max <= t_min) return false;
	}

	rec.t = t_min;
	rec.p = r.at(rec.t);
	Vector3 outward_normal = Vector3(0,0,0);
	rec.set_face_normal(r, outward_normal);
	rec.material_ptr = material;

	return true;
}

#endif