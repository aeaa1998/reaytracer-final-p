//
// Created by Augusto Alonso on 10/8/20.
//
#ifndef COLOR_H
#define COLOR_H


#include "vec3.h"
#include "vector"
#include <iostream>

bool compare(ColorContainer a, ColorContainer b)
{
    if (a.x > a.y){
        return true;
    } else if (a.x < a.y){
        return false;
    }else{
        return a.y > b.y;
    }
}

ColorContainer write_color(color pixel_color, int x, int y, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);
    pixel_color.setX(r);
    pixel_color.setY(g);
    pixel_color.setZ(b);
    ColorContainer cont;
    cont.color = pixel_color;
    cont.x = x;
    cont.y = y;
    return cont;
//    return [r];
    // Write the translated [0,255] value of each color component.
//    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
//        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
//        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
//-std=c++14

#endif
void savebmp (const char *filename, int w, int h, int dpi, std::vector<ColorContainer> pixel_colors) {
    FILE *f;
    int k = w*h;
    int s = 4*k;
    int filesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    bmpfileheader[ 2] = (unsigned char)(filesize);
    bmpfileheader[ 3] = (unsigned char)(filesize>>8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(w);
    bmpinfoheader[ 5] = (unsigned char)(w>>8);
    bmpinfoheader[ 6] = (unsigned char)(w>>16);
    bmpinfoheader[ 7] = (unsigned char)(w>>24);

    bmpinfoheader[ 8] = (unsigned char)(h);
    bmpinfoheader[ 9] = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    f = fopen(filename,"wb");

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
////    std::cout << "P3\n" << pixel_colors.size() <<  "\n255\n";

    for (int i = 0; i < k; i++) {
        color pixel_color = pixel_colors[pixel_colors.size() - 1 - i].color;
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();
        double red = static_cast<int>(256 * clamp(r, 0.0, 0.999));
        double green = static_cast<int>(256 * clamp(g, 0.0, 0.999));
        double blue = static_cast<int>(256 * clamp(b, 0.0, 0.999));

        unsigned char color[3] = {static_cast<unsigned char>((int)floor(blue)),static_cast<unsigned char>((int)floor(green)),static_cast<unsigned char>((int)floor(red))};

        fwrite(color,1,3,f);
    }

    fclose(f);
}

