//
// Created by Augusto Alonso on 10/6/20.
//
#ifndef TEXTURE_H
#define TEXTURE_H

#include "utils.h"

#include "perlin.h"
#include "rtw_stb_image.h"

#include <iostream>


class Texture  {
    public:
        virtual color value(double u, double v, const vec3& p) const = 0;
};


class SolidColor : public Texture {
    public:
        SolidColor() = default;
        SolidColor(color c) : color_value(c) {}

        SolidColor(double red, double green, double blue)
          : SolidColor(color(red, green, blue)) {}

        virtual color value(double u, double v, const vec3& p) const override {
            return color_value;
        }

    private:
        color color_value;
};


class NoiseTexture : public Texture {
    public:
        NoiseTexture() {}
        NoiseTexture(double sc) : scale(sc) {}

        virtual color value(double u, double v, const vec3& p) const override {
            return color(1,1,1) * noise.noise(p);
            // return color(1,1,1)*0.5*(1 + noise.turb(scale * p));
            // return color(1,1,1)*noise.turb(scale * p);
//            return color(1,1,1)*0.5*(1 + sin(scale*p.z() + 10*noise.turb(p)));
        }

    public:
        perlin noise;
        double scale;
};

#endif
