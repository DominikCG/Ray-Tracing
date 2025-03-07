#include <iostream>
#include <fstream>
#include "Vec3.h"
#include "color.h"
#include "ray.h"
#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "Camera.h"
#include "Material.h"

color ray_color(const ray& r, const hittable& world, int depth){

	hit_record rec;
	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}
	//else{ 
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	//}



}
double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
}




int main() {
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 384;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 1;
	const int max_depth = 50;

	std::ofstream outfile;
	outfile.open("imagem.ppm", std::ofstream::out | std::ofstream::trunc);
	outfile<< "P3\n" << image_width << " " << image_height << "\n255\n";

	point3 origin(0.0, 0.0, 0.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.25, 0.0);
	point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, 1);


	hittable_list world;

	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.7, 0.3, 0.3))));

	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));

	world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<lambertian>(color(.8, .6, .2))));
	world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<lambertian>(color(.8, .8, .8))));

	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));

	point3 lookfrom(0, 0, 0);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);


	camera cam(lookfrom, lookat, vup, 100, aspect_ratio);


	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(outfile, pixel_color, samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
	outfile.close();
	system("pause");
}