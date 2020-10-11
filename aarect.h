//
// Created by Augusto Alonso on 10/6/20.
//


#ifndef UNTITLED2_AARECT_H
#define UNTITLED2_AARECT_H


#include "utils.h"

#include "Shape.h"


class XY_Plane : public Shape {
public:
    XY_Plane() {}

    XY_Plane(
            double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat
    ) : x_min(_x0), x_max(_x1), y_min(_y0), y_max(_y1), k(_k), mp(mat) {};

    bool hit(const Ray& r, double t0, double t1, Record& rec) const override;

    bool bounding_box(double t0, double t1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Z
        // dimension a small amount.
        output_box = AABB(Vector3(x_min, y_min, k - 0.0001), Vector3(x_max, y_max, k + 0.0001));
        return true;
    }

public:
    shared_ptr<Material> mp;
    double x_min, x_max, y_min, y_max, k;
};

class XZ_Plane : public Shape {
public:
    XZ_Plane() {}

    XZ_Plane(
            double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<Material> mat
    ) : x_min(_x0), x_max(_x1), z_min(_z0), z_max(_z1), k(_k), mp(mat) {};

    bool hit(const Ray& r, double t0, double t1, Record& rec) const override;

    bool bounding_box(double t0, double t1, AABB& output_box) const override {
        // The bounding box must have non-zero width in each dimension, so pad the Y
        // dimension a small amount.
        output_box = AABB(Vector3(x_min, k - 0.0001, z_min), Vector3(x_max, k + 0.0001, z_max));
        return true;
    }

public:
    shared_ptr<Material> mp;
    double x_min, x_max, z_min, z_max, k;
};

class YZ_Plane : public Shape {
public:
    YZ_Plane() = default;

    YZ_Plane(
            double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<Material> mat
    ) : y_min(_y0), y_max(_y1), z_min(_z0), z_max(_z1), k(_k), mp(mat) {};

    bool hit(const Ray& r, double t0, double t1, Record& rec) const override;

    virtual bool bounding_box(double t0, double t1, AABB& output_box) const override {
        output_box = AABB(Vector3(k - 0.0001, y_min, z_min), Vector3(k + 0.0001, y_max, z_max));
        return true;
    }

public:

    shared_ptr<Material> mp;
    double y_max, z_min, z_max, k;
    double y_min;
};

bool XY_Plane::hit(const Ray& r, double t0, double t1, Record& rec) const {
    auto t = (k-r.origin().z()) / r.direction().z();
    if (t < t0 || t > t1)
        return false;

    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x_min || x > x_max || y < y_min || y > y_max)
        return false;

    rec.u = (x - x_min) / (x_max - x_min);
    rec.v = (y - y_min) / (y_max - y_min);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

bool XZ_Plane::hit(const Ray& r, double t0, double t1, Record& rec) const {
    auto t = (k-r.origin().y()) / r.direction().y();
    if (t < t0 || t > t1)
        return false;

    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < x_min || x > x_max || z < z_min || z > z_max)
        return false;

    rec.u = (x - x_min) / (x_max - x_min);
    rec.v = (z - z_min) / (z_max - z_min);
    rec.t = t;
    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

bool YZ_Plane::hit(const Ray& r, double t0, double t1, Record& rec) const {
    auto t = (k-r.origin().x()) / r.direction().x();
    if (t < t0 || t > t1)
        return false;

    auto y = r.origin().y() + t*r.direction().y();
    auto z = r.origin().z() + t*r.direction().z();
    if (y < y_min || y > y_max || z < z_min || z > z_max)
        return false;

    rec.u = (y-y_min)/(y_max - y_min);
    rec.v = (z - z_min) / (z_max - z_min);
    rec.t = t;
    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

#endif //UNTITLED2_AARECT_H