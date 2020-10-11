#ifndef MATERIAL_H
#define MATERIAL_H
//
// Created by Augusto Alonso on 10/6/20.
//
#include "utils.h"

#include "shapes/Shape.h"
#include "Texture.h"


double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}


class Material {
public:
    virtual color emitted(double u, double v, const Vector3& p) const {
        return color(0,0,0);
    }

    virtual bool scatter(
            const Ray& r_in, const Record& rec, color& attenuation, Ray& scattered
    ) const = 0;
};


class lambertian : public Material {
public:
    lambertian(const color& a) : albedo(make_shared<SolidColor>(a)) {}
    lambertian(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(
            const Ray& r_in, const Record& rec, color& attenuation, Ray& scattered
    ) const override {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = Ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<Texture> albedo;
};


class metal : public Material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
            const Ray& r_in, const Record& rec, color& attenuation, Ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};


class dielectric : public Material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
            const Ray& r_in, const Record& rec, color& attenuation, Ray& scattered
    ) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = Ray(rec.p, direction, r_in.time());
        return true;
    }

public:
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};


class diffuse_light : public Material {
public:
    diffuse_light(shared_ptr<Texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<SolidColor>(c)) {}

    virtual bool scatter(
            const Ray& r_in, const Record& rec, color& attenuation, Ray& scattered
    ) const override {
        return false;
    }

    virtual color emitted(double u, double v, const Vector3& p) const override {
        return emit->value(u, v, p);
    }

public:
    shared_ptr<Texture> emit;
};


class isotropic : public Material {
public:
    isotropic(color c) : albedo(make_shared<SolidColor>(c)) {}
    isotropic(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(
            const Ray& r_in, const Record& rec, color& attenuation, Ray& scattered
    ) const override {
        scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

public:
    shared_ptr<Texture> albedo;
};


#endif
