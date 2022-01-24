

#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class Scene : public Object {
public:
	Scene() {}
	Scene(shared_ptr<Object> obj) { add(obj); }

	void clear() {}
	void add(shared_ptr<Object> obj) { objects.push_back(obj); }

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

public:
	std::vector<shared_ptr<Object>> objects;
};


bool Scene::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	HitRecord temp_rec;
	bool hits = false;
	auto closest = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest, temp_rec)) {
			hits = true;
			closest = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hits;
}

#endif
