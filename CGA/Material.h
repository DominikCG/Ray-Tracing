#ifndef MATERIALH
#define MATERIALH


#include "ray.h"
#include "hittable.h"
#include "Hittable_list.h"
#include "Vec3.h"


struct hit_record;


float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}


bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}


vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n)*n;
}


vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
	} while (p.length_squared() >= 1.0);
	return p;
}


class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};


class lambertian : public material {
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};
//class dielectric : public material {
//public:
//	dielectric(double ri) : ref_idx(ri) {}
//
//	virtual bool scatter(
//		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
//	) const {
//		attenuation = color(1.0, 1.0, 1.0);
//		double etai_over_etat;
//		if (rec.front_face) {
//			etai_over_etat = 1.0 / ref_idx;
//		}
//		else {
//			etai_over_etat = ref_idx;
//		}
//
//		vec3 unit_direction = unit_vector(r_in.direction());
//		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
//		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
//		if (etai_over_etat * sin_theta > 1.0) {
//			vec3 reflected = reflect(unit_direction, rec.normal);
//			scattered = ray(rec.p, reflected);
//			return true;
//		}         
//		double reflect_prob = schlick(cos_theta, etai_over_etat);
//		if (random_double() < reflect_prob)
//		{
//			vec3 reflected = reflect(unit_direction, rec.normal);
//			scattered = ray(rec.p, reflected);
//			return true;
//		}
//
//		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
//		scattered = ray(rec.p, refracted);
//		return true;
//	}
//
//	double ref_idx;
//};

class metal : public material
{
public:
	 metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool Scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 albedo;
	float fuzz;
};



#endif