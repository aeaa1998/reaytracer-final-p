#ifndef BOX_H
#define BOX_H
//
// Created by Augusto Alonso on 10/8/20.
//


#include "utils.h"

#include "aarect.h"
#include "Scene.h"


class box : public Shape  {
public:
    box() = default;
    box(const Vector3& p0, const Vector3& p1, const shared_ptr<Material>& ptr);

    bool hit(const Ray& r, double t0, double t1, Record& rec) const override;

    bool bounding_box(double t0, double t1, AABB& output_box) const override {
        output_box = AABB(box_min, box_max);
        return true;
    }

public:
    Vector3 box_min;
    Vector3 box_max;
    Scene sides;
};


box::box(const Vector3& p0, const Vector3& p1, const shared_ptr<Material>& ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<XY_Plane>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(make_shared<XY_Plane>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(make_shared<XZ_Plane>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(make_shared<XZ_Plane>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(make_shared<YZ_Plane>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(make_shared<YZ_Plane>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const Ray& r, double t0, double t1, Record& rec) const {
    return sides.hit(r, t0, t1, rec);
}


#endif
