//
// Created by Augusto Alonso on 10/7/20.
//

#ifndef UNTITLED2_TRIANGLE_H
#define UNTITLED2_TRIANGLE_H


#include "vec3.h"
#include "Shape.h"
#include "material.h"
#include "Ray.h"
#define K_EPSILON 0.00001
class Triangle : public Shape  {
public:
    Triangle() = default;
    Triangle(vec3 p1,vec3 p2,vec3 p3, shared_ptr<Material> m)
            : p1(p1), p2(p2), p3(p3), mat_ptr(m) {};

    bool hit(
            const Ray& r, double tmin, double tmax, Record& rec) const override;

     bool bounding_box(double t0, double t1, AABB& output_box) const override;

public:
    Vector3 p1,p2,p3;
    shared_ptr<Material> mat_ptr;

    Vector3 getNormal(const Vector3 &hitPoint) const;
};


bool Triangle::bounding_box(double t0, double t1, AABB& output_box) const {

    return true;
}


Vector3 Triangle::getNormal(const Vector3 &hitPoint) const {
    Vector3 v01 = p2 - p1;
    Vector3 v02 = p3 - p1;

    Vector3 N = cross(v01, v02);

    return normalize(N);
}

bool Triangle::hit(const Ray& r, double t_min, double t_max, Record& rec) const {

    Vector3 N = getNormal(Vector3());

    auto normalDotRd = dot(N, r.direction());
    if(fabs(normalDotRd) <= 0) {
        return false; // Ray and plane are parallel
    }
    auto d = dot(N, p1);

    // Compute t
    auto NdotRo = dot(N, r.origin());
    t_min = - ((NdotRo + d) / normalDotRd);
    if(t_min < 0) return false; // Triangle is behind Ray

    Vector3 hitPoint = r.origin() + r.direction() * t_min;


    Vector3 C;

    Vector3 v01 = p2 - p1;
    Vector3 v12 = p3 - p2;
    Vector3 v20 = p1 - p3;

    Vector3 vp0 = hitPoint - p1;
    C = cross(v01, vp0);
    if (dot(N, C) < 0.0) return false;

    Vector3 vp1 = hitPoint - p2;
    C = cross(v12, vp1);
    if (dot(N, C) < 0.0) return false;

    Vector3 vp2 = hitPoint - p3;
    C = cross(v20, vp2);
    if (dot(N, C) < 0.0) return false;

    return true; // Triangle intersects Ray
}


#endif //UNTITLED2_TRIANGLE_H