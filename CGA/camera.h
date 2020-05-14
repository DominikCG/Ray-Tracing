#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:

	camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, // vertical field-of-view in degrees
		   double aspect_ratio) {
		origin = lookfrom;

		auto theta = degrees_to_radians(vfov);
		auto half_height = tan(theta / 2);
		auto half_width = aspect_ratio * half_height;
		vec3 u, v, w;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * u - half_height * v - w;

		horizontal = 2 * half_width*u;
		vertical = 2 * half_height*v;

		lower_left_corner = point3(-half_width, -half_height, -1.0);

		horizontal = vec3(2 * half_width, 0.0, 0.0);
		vertical = vec3(0.0, 2 * half_height, 0.0);
	}

	camera() {
		lower_left_corner = point3(-2.0, -1.0, -1.0);
		horizontal = vec3(4.0, 0.0, 0.0);
		vertical = vec3(0.0, 2.0, 0.0);
		origin = point3(0.0, 0.0, 0.0);
	}

	ray get_ray(double u, double v) const {
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
};
#endif