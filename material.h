

#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
#include "vector3.h"
#include "object.h"
#include "texture.h"

struct HitRecord;

class Material {
public:
	virtual bool scatter(
		const Ray& ray_in, const HitRecord& rec, Colour& attenuation, Ray& scattered
	) const = 0;

	virtual Colour emitted() const {
		return Colour(0, 0, 0);
	}
};

class Lambertian : public Material {
public:
	Lambertian(const Colour& c) : albedo(make_shared<SolidColour>(c)) {}
	Lambertian(shared_ptr<Texture> t) : albedo(t) {}
	
	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
public:
	shared_ptr<Texture> albedo;
};


class Metal : public Material {
public:
	Metal(const Colour& c, double f) : albedo(c), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const Ray& ray_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override {
		Vector3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
public:
	Colour albedo;
	double fuzz;
};



class Dielectric : public Material {
public:
	Dielectric(double refractive_index) : ir(refractive_index) {}

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Colour& attenuation, Ray& scattered) const override {
		attenuation = Colour(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		Vector3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		Vector3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = Ray(rec.p, direction);
		return true;
	}

private:
	double ir;

	static double reflectance(double cosine, double ir) {
		auto r0 = (1 - ir) / (1 + ir);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

};



class Light : public Material {
public:
	Light() {}
	Light(Colour c) : colour(c) {};


	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Colour& atten, Ray& scattered) const override {
		return false;
	}

	virtual Colour emitted() const {
		return colour;
	}

public:
	Colour colour;
};


#endif