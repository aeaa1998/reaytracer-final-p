//
// Created by Augusto Alonso on 10/6/20.
//
#ifndef SPHERE_H
#define SPHERE_H
#include "../utils.h"

#include "Shape.h"


class Sphere : public Shape  {
public:
    Sphere() {}
    Sphere(Vector3 cen, double r, shared_ptr<Material> m)
            : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(
            const Ray& r, double tmin, double tmax, Record& rec) const override;

    virtual bool bounding_box(double t0, double t1, AABB& output_box) const override;

public:
    Vector3 center;
    double radius;
    shared_ptr<Material> mat_ptr;
};


bool Sphere::bounding_box(double t0, double t1, AABB& output_box) const {
    output_box = AABB(
            center - vec3(radius, radius, radius),
            center + vec3(radius, radius, radius));
    return true;
}


void get_sphere_uv(const Vector3& p, double& u, double& v) {
    auto phi = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u = 1-(phi + pi) / (2*pi);
    v = (theta + pi/2) / pi;
}


bool Sphere::hit(const Ray& r, double t_min, double t_max, Record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}


#endif
