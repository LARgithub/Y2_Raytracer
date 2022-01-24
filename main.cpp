
#include <iostream>
#include <fstream>
#include <iomanip>

#include "material.h"
#include "utility.h"
#include "object.h"
#include "sphere.h"
#include "scene.h"
#include "cube.h"
#include "camera.h"

/*
// Old intersection testing, only determines whether sphere has been intersected or not
bool sphere_intersection(const Point& center, double radius, const Ray& r) {
	Vector3 OC = r.origin() - center;

	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(OC, r.direction());
	auto c = dot(OC, OC) - radius * radius;

	auto discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}
/*
// (Old) New intersection test returns normal value
double sphere_intersection(const Point& center, double radius, const Ray& r) {
	Vector3 OC = r.origin() - center;
	
	// Simplified quadratic calculations
	auto a = r.direction().length_squared();
	auto half_b = dot(OC, r.direction());
	auto c = OC.length_squared() - radius * radius;

	auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
}
*/

Colour ray_colour(const Ray& r, const Scene& world, const Colour& bg, int depth) {
	HitRecord rec;

	// If exceeded ray depth, return black
	if (depth <= 0) {
		return Colour(0, 0, 0);
	}

	// If the ray hits nothing, return the background color.
	if (!world.hit(r, 0.001, inf, rec))
		return bg;

	Ray scattered;
	Colour attenuation;
	Colour emitted = rec.material_ptr->emitted();

	if (!rec.material_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_colour(scattered, world, bg, depth - 1);

	/*
	// Initial colouring, only if sphere hit
	if (sphere_intersection(Point(0, 0, -1), 0.5, r)) {
		return Colour(1, 0, 0);
	}

	//Second colouring, colour is based upon surface normal
	auto t = sphere_intersection(Point(0, 0, -1), 0.5, r);

	if (t > 0.0) {
		Vector3 normal = unit_vector(r.at(t) - Vector3(0, 0, -1));

		// Colour dependant on normal from hit
		return 0.5 * Colour(normal.x() + 1, normal.y() + 1, normal.z() + 1); 
	}

	Vector3 unit_direction = unit_vector(r.direction());
	t = 0.5 * (unit_direction.y() + 1.0);
	auto t = 0.5 * (unit_direction.y() + 1.0); // Colour dependant on Y coordinate of r
	
	* 
	* Old ray colour code

	if (world.hit(r, 0.001, inf, rec)) {
		Ray scattered;
		Colour attenuation;

		if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_colour(scattered, world, depth - 1);
		return Colour(0, 0, 0);

		Point target = rec.p + rec.normal + random_unit_vector();
		return 0.5 * ray_colour(Ray(rec.p, target-rec.p), world, depth);
	}

	Vector3 unit_dir = unit_vector(r.direction());
	auto t = 0.5 * (unit_dir.y() + 1.0);

	// Apply linear interpolation
	// t = 1.0 -> Blue. t = 0.0 -> White
	return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
	/**/
}


void write_colour(std::ostream& out, Colour pixel_colour, int samples) {
	/*
	out << static_cast<int>(255.999 * pixel_colour.x()) << ' '
		<< static_cast<int>(255.999 * pixel_colour.y()) << ' '
		<< static_cast<int>(255.999 * pixel_colour.z()) << '\n';
	*/

	auto r = pixel_colour.x();
	auto g = pixel_colour.y();
	auto b = pixel_colour.z();

	auto scale = 1.0 / samples;
	r *= scale;
	g *= scale;
	b *= scale;

	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';

}


Scene test_scene(){
	//// Scene
	// Define some materials
	auto material_floor = make_shared<Checkerboard>(Colour(0.1,0,0.1), Colour(0.5,1,0.7), 5);

	auto material_right = make_shared<PerlinNoise>(3);

	auto material_left = make_shared<Lambertian>(Colour(0.0, 0.5, 1));
	auto material_glass = make_shared<Dielectric>(0.9);
	auto material_behind = make_shared<Lambertian>(Colour(1.0, 1.0, 0));

	auto material_light = make_shared<Light>(Colour(50,50,50));
	auto material_light_blue = make_shared<Light>(Colour(0,0,10));
	auto material_light_red = make_shared<Light>(Colour(10, 0, 0));

	// Create and populate scene
	Scene world;
	world.add(make_shared<Sphere>(Point(0, 0, -1), 0.5, material_glass));
	
	world.add(make_shared<Cube>(Point(1, -0.5, -2), Point(2,0.5,-3), material_left));

	world.add(make_shared<Sphere>(Point(-1, 0, -2), 0.5, make_shared<Lambertian>(material_right)));
	world.add(make_shared<Sphere>(Point(0, -100.5, -2), 100, make_shared<Lambertian>(material_floor)));
	world.add(make_shared<Sphere>(Point(0, 0, -3), 0.5, material_behind));

	world.add(make_shared<Sphere>(Point(0, 7.5, -1), 1, material_light));

	return world;
}


int main() {
	std::cout << "Hello World" << std::endl;

	// Ray Properties
	const int samples_per_pixel = 1000;
	const int max_ray_depth = 75;

	// Image Properties
	const auto aspect_ratio = 16.0 / 9.0;
	const int img_W = 600;
	const int img_H = static_cast<int>(img_W / aspect_ratio);

	Camera cam(Point(-1,1.5,2), Point(0,0,-1), Point(0,1,0), 40, aspect_ratio);

	Scene world = test_scene();

	auto bg = Colour(0,0,0);

	// File to write to
	std::ofstream imgfile;
	imgfile.open("imgs/img_output-demo_output.ppm");
	imgfile << "P3\n" << img_W << ' ' << img_H << "\n255\n";


	float pct;	
	for (int j = img_H - 1; j >= 0; j--) {
		pct = (((float)(img_H - j) / (float)img_H) * 100);
		std::cerr << "\rRows remaining: " << j << ' ' << "| " << std::setprecision(3) << pct << "% Complete         " << std::flush;
		for (int i = img_W - 1; i >= 0; i--) {
			/*
			auto R = double(i) / (img_W - 1);
			auto G = double(j) / (img_H - 1);
			auto B = 0.25;

			// Cast to INT for PPM format
			int iR = static_cast<int>(255.999 * R);
			int iG = static_cast<int>(255.999 * G);
			int iB = static_cast<int>(255.999 * B);

			imgfile << iR << ' ' << iG << ' ' << iB << '\n';

			// Use pixel coordinate for Colour
			Colour pixel_colour(
				double(i) / (img_W - 1),
				double(j) / (img_H - 1),
				0.25
			);
			*/

			Colour pixel_colour = Colour(0,0,0);

			// Sample same pixel multiple times at multiple points and get an average



// Previously, I used OpenMP to make use of the multiple threads available on my CPU, however this was still quite slow
//#pragma omp parallel loop

// Changing to the MinGW64 port of the GCC compiler allows me to use OpenACC, enabling GPU acceleration for designated sections
// This section is calculating an average colour over the many samples, which is very easily parallelized
#pragma acc kernel private(max_ray_depth, pixel_colour)
			for (int s = 0; s < samples_per_pixel; s++) {
				// Transform pixel position to viewport position
				// Add random value to average over all samples
				auto u = double(i + random_double()) / (img_W - 1);
				auto v = double(j + random_double()) / (img_H - 1);

				// Cast a ray to viewport position (u,v)
				Ray ray = cam.get_ray(u,v);

				pixel_colour += ray_colour(ray, world, bg, max_ray_depth);
			}

			write_colour(imgfile, pixel_colour, samples_per_pixel);

		}
	}
	
	std::cout << "\nImage complete" << std::endl;
	return 0;
}