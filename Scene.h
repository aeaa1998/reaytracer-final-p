//
// Created by Augusto Alonso on 10/6/20.
//
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "utils.h"

#include "Shape.h"

#include <vector>


class Scene : public Shape  {
public:
    Scene() {}
    Scene(shared_ptr<Shape> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Shape> object) { objects.push_back(object); }

    virtual bool hit(
            const Ray& r, double tmin, double tmax, Record& rec) const override;

    virtual bool bounding_box(double t0, double t1, AABB& output_box) const override;

public:
    std::vector<shared_ptr<Shape>> objects;
};


bool Scene::hit(const Ray& r, double t_min, double t_max, Record& rec) const {
    Record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}


bool Scene::bounding_box(double t0, double t1, AABB& output_box) const {
    if (objects.empty()) return false;

    AABB temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(t0, t1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}


#endif
