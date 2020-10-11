//
// Created by Augusto Alonso on 10/6/20.
//
#ifndef RAY_H
#define RAY_H

#include "vec3.h"


class Ray {
    public:
        Ray() {}
        Ray(const Vector3& origin, const vec3& direction)
            : orig(origin), dir(direction), tm(0)
        {}

        Ray(const Vector3& origin, const vec3& direction, double time)
            : orig(origin), dir(direction), tm(time)
        {}

        Vector3 origin() const  { return orig; }
        vec3 direction() const { return dir; }
        double time() const    { return tm; }

        Vector3 at(double t) const {
            return orig + t*dir;
        }


    public:
        Vector3 orig;
        vec3 dir;
        double tm;
};

#endif
