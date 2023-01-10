#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "vec3.h"





class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 cen, double r, shared_ptr<material> m): center(cen), radius(r), mat_ptr(m) {};
	//sphere(point3 cen, double r) : center(cen), radius(r) {};
	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const
		override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box)
		const override;


public:
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;

private:
	static void get_sphere_uv(const point3& p, double& u, double& v) {
		// p: a given point on the sphere of radius one, centered at the
		//origin.
			// u: returned value [0,1] of angle around the Y axis from X=-1.
			// v: returned value [0,1] of angle from Y=-1 to Y=+1.
			// <1 0 0> yields <0.50 0.50> <-1 0 0> yields <0.00
			//0.50 >
			// <0 1 0> yields <0.50 1.00> < 0 -1 0> yields <0.50
			//0.00 >
			// <0 0 1> yields <0.25 0.50> < 0 0 -1> yields <0.75
			//0.50 >
			auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + pi;
		u = phi / (2 * pi);
		v = theta / pi;
	}

};




bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec)
const {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);
	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}
	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);

	get_sphere_uv(outward_normal, rec.u, rec.v);

	rec.mat_ptr = mat_ptr;


	return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
	output_box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}

#endif