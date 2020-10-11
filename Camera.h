//
// Created by Augusto Alonso on 10/8/20.
//

#ifndef CAMERA_H
#define CAMERA_H


#include "utils.h"


class Camera {
    public:
        Camera() : Camera(Vector3(0, 0, -1), Vector3(0, 0, 0), vec3(0, 1, 0), 40, 1, 0, 10) {}

        Camera(
                Vector3 lookfrom,
                Vector3 lookat,
                vec3   vup,
                double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
                double aperture,
                double focus_dist,
                double t0 = 0,
                double t1 = 0
        ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

            lens_radius = aperture / 2;
            time0 = t0;
            time1 = t1;
        }

        Ray get_ray(double s, double t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            return Ray(
                origin + offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset,
                random_double(time0, time1)
            );
        }

    private:
        Vector3 origin;
        Vector3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lens_radius;
        double time0, time1;  // shutter open/close times
};

#endif
