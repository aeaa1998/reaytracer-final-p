//
// Created by Augusto Alonso on 10/8/20.
//

#include "utils.h"


#include "Camera.h"
#include "shapes/aarect.h"
#include "color.h"
#include "Scene.h"
#include "material.h"
#include "shapes/Sphere.h"
#include "shapes/triangle.h"
#include <pthread.h>
#include "shapes/box.h"
#include "omp.h"
//#include "omp-tools.h"
//#include "ompt.h"
#include "vector"

#include <iostream>


#define NUM_THREADS 2
int image_width = 0;
int image_height = 0;
int max_depth = 0;
int samples_per_pixel = 0;
Camera cam;
color background;
std::vector<color> colors;
std::vector<ColorContainer>  colorsContainers;
std::mutex values_mutex;
Scene world;
color pixel_color;

void values_push_back(ColorContainer c)
{
    values_mutex.lock();
    colorsContainers.push_back(c);
    values_mutex.unlock();
}

color ray_color(const Ray& r, const color& background, const Shape& world, int depth) {
    try {
        Record rec;

        // If we've exceeded the Ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);

        // If the Ray hits nothing, return the background color.
        if (!world.hit(r, 0.001, infinity, rec))
            return background;

        Ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted;

        return emitted + attenuation * ray_color(scattered, background, world, depth-1);
    } catch(int e){
        return color(0,0,0);
    }

}

double moveRight(double a){
    return a;
}

double moveLeft(double a){
    return a * -1;
}

Scene addTree(Scene objects, int xOffset = 0, int yOffset = 0, int zOffset = 0, double scale = 1.0){
    auto log = make_shared<lambertian>(color(102/255.0, 51/255.0, 0/242.0));
    auto treeColor = make_shared<lambertian>(color(0/255.0, 128/255.0, 0/242.0));
    shared_ptr<Shape> box1 = make_shared<box>(Vector3(0, 0, 0), Vector3(40 * scale, 100 * scale, 65 * scale), log);
    shared_ptr<Shape> orginalTree = make_shared<box>(Vector3(-40 * scale, 0, -40 * scale), Vector3(80 * scale, 40 * scale, 80 * scale), treeColor);
    shared_ptr<Shape> orginalTree2 = make_shared<box>(Vector3(-80 * scale, 0, -80 * scale), Vector3(120 * scale, 40 * scale, 120 * scale), treeColor);
    auto tree1 = make_shared<Translate>(orginalTree, vec3(265 + xOffset, 100 * scale + yOffset, 295 + zOffset));
    auto tree2 = make_shared<Translate>(orginalTree2, vec3(265 + xOffset, 140 * scale + yOffset, 295 + zOffset));
    auto tree3 = make_shared<Translate>(orginalTree, vec3(265 + xOffset, 180 * scale + yOffset, 295 + zOffset));
    box1 = make_shared<Translate>(box1, vec3(265 + xOffset, 0 + yOffset, 295 + zOffset));
    objects.add(tree1);
    objects.add(tree2);
    objects.add(tree3);
    objects.add(box1);
    return objects;
}

Scene makeMountains(Scene objects, int ctyX, int ctY, int snowY, int initialOffsetX = 0, int initialOffsetY = 0){
    auto stone = make_shared<metal>(color(136/255.0, 140/255.0, 141/242.0), 1.0);
    auto snow = make_shared<isotropic>(color(1, 1, 1));
    shared_ptr<Shape> stoneCube = make_shared<box>(Vector3(0, 0, 0), Vector3(60, 60, 60), stone);
    shared_ptr<Shape> snowCube = make_shared<box>(Vector3(0, 0, 0), Vector3(60, 60, 60), snow);
    for (int i = initialOffsetX; i < ctyX + initialOffsetX; ++i) {
        for (int j = initialOffsetY; j < ctY + initialOffsetY; ++j) {
            auto xOffset = 60 * i;
            auto yOffset = 60 * j;
            auto zOffset = 60 *  random_double();
            auto c = vec3(-190 + xOffset ,20 + yOffset ,460 + zOffset);
            if (j >= snowY){
                if (random_double() <= .5){
                    auto stoneCube1 = make_shared<Translate>(snowCube, c);
                    objects.add(stoneCube1);
                }else{
                    auto stoneCube1 = make_shared<Translate>(stoneCube, c);
                    objects.add(stoneCube1);
                }

            }else{
                auto stoneCube1 = make_shared<Translate>(stoneCube, c);
                objects.add(stoneCube1);
            }

        }
    }
    return objects;
}

Scene final_scene() {
    Scene objects;


    auto glassMirror = make_shared<dielectric>(1.5);
    auto waterMirror = make_shared<dielectric>(2.5);

    auto ground = make_shared<lambertian>(color(155/255.0, 118/255.0, 93/242.0));

    auto light = make_shared<diffuse_light>(color(15,15,15));
    auto blueSky = make_shared<isotropic>(color(56.0/255.0,176/255.0,222/255.0));
    auto dungeon = make_shared<isotropic>(color(99/255.0,99/255.0,99/255.0));
    auto blueSkyLight = make_shared<diffuse_light>(color(90/255.0,188/255.0,216/255.0));


//    LIGHT
    objects.add(make_shared<XZ_Plane>(0, 500, -100, 400, 800, light));


    auto stone = make_shared<metal>(color(136/255.0, 140/255.0, 141/242.0), 1.0);
    auto snow = make_shared<isotropic>(color(1, 1, 1));

//PIG
    auto pigPink = color(253/255.0,53/255.0,108/255.0);
    auto pigMetal = make_shared<lambertian>(pigPink);
    objects.add(make_shared<Sphere>(Vector3(175, 55, 100), 20.0, pigMetal));
    objects.add(make_shared<Sphere>(Vector3(155, 45, 100), 7.0, pigMetal));
    objects.add(make_shared<Sphere>(Vector3(215, 40, 100), 30.0, pigMetal));
    objects.add(make_shared<Sphere>(Vector3(235, 40, 100), 30.0, pigMetal));
    objects.add(make_shared<Sphere>(Vector3(205, 1.5, 80), 10.0, pigMetal));
    objects.add(make_shared<Sphere>(Vector3(195, 1, 120), 10.0, pigMetal));
    objects.add(make_shared<Sphere>(Vector3(245, 1.5, 80), 10.0, pigMetal));
    objects.add(make_shared<Sphere>(Vector3(235, 1, 120), 10.0, pigMetal));
//WITH REFRACTION bubbles

    objects.add(make_shared<Sphere>(Vector3(75, 400, 400), 70.0, glassMirror));
    objects.add(make_shared<Sphere>(Vector3(-75, 400, 200), 70.0, glassMirror));
    objects.add(make_shared<Sphere>(Vector3(175, 450, 350), 80.0, glassMirror));
    auto pertext = make_shared<NoiseTexture>(4);
    //SetUp Env
    objects.add(make_shared<XZ_Plane>(-500, 1000, 800, 1055, 1, make_shared<lambertian>(pertext)));
    objects.add(make_shared<XZ_Plane>(-500, 150, -200, 1055, 0, waterMirror));
    objects.add(make_shared<XZ_Plane>(150, 1000, -200, 1055, 0, ground));
    objects.add(make_shared<XZ_Plane>(-500, 1000, -200, 1055, -20, blueSkyLight));
//BG
    objects.add(make_shared<XY_Plane>(-350, 815, -800, 800, 560, dungeon));
//ADDING Mountaint
        objects = makeMountains(objects, 15, 2, 2);
    objects = makeMountains(objects, 13, 2, 0, 1, 2);
    objects = makeMountains(objects, 8, 2, 0, 3, 4);
//ADDING TREES
    objects = addTree(objects);
    objects = addTree(objects, 90, 0, -87, 0.5);
    objects = addTree(objects, 220, 0, -147, 0.5);
    objects = addTree(objects, 220, 0, 84, 0.5);
    objects = addTree(objects, 390, 0, 77, 0.4);
    objects = addTree(objects, 400, 0, 147, 0.4);
    objects = addTree(objects, 520, 0, 184, 0.4);
    return objects;
}






int main() {

    // Image
    clock_t t1, t2;
    t1 = clock();
    const auto aspect_ratio = 1 / 1;
    image_width = 100;
    image_height = static_cast<int>(image_width / aspect_ratio);
    samples_per_pixel = 1;
    max_depth = 4000;

    world = final_scene();

    // Camera
    colors.resize(image_width * image_height);
    colorsContainers.resize(image_width * image_height);
    pthread_t tID[samples_per_pixel];
    int t[samples_per_pixel];
//    struct mmm lolo;
    Vector3 lookfrom(278, 278, -800);
    Vector3 lookat(278, 278, 0);
    vec3 vup(0,1,0);
    pthread_attr_t attr;
    void *status;


    auto dist_to_focus = 100.0;
    auto aperture = 0.0;
    double vfov = 40.0;
    int rc;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    cam = Camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render
//    omp_set_dynamic(0);     // Explicitly disable dynamic teams
//    omp_set_num_threads(image_height);
//    omp_set_nested(true);
//    #pragma omp parallel for
//    {
//        int j = omp_get_thread_num();
        for (int i_height = image_height - 1; i_height >= 0; --i_height) {
            std::cerr << "\rLineas faltantes: " << i_height << ' ' << std::flush;
            for (int i_width = 0; i_width < image_width; ++i_width) {
                pixel_color = color(0, 0, 0);

                if (samples_per_pixel > 0) {
                    for (int s = 0; s < samples_per_pixel; ++s) {
                        auto u = (i_width + random_double()) / (image_width - 1);
                        auto v = (i_height + random_double()) / (image_height - 1);
                        Ray r = cam.get_ray(u, v);
                        pixel_color += ray_color(r, background, world, max_depth);

                    }

                } else {
                    Vector3 gottenColor;
                    Ray r;
                    do {
                        auto u = (i_width + random_double()) / (image_width - 1);
                        auto v = (i_height + random_double()) / (image_height - 1);
                        r = cam.get_ray(u, v);
                        gottenColor = ray_color(r, background, world, max_depth);
                    } while (gottenColor.x() == 0 && gottenColor.y() == 0 && gottenColor.z() == 0);

                    pixel_color += ray_color(r, background, world, max_depth);

                }
//                colorsContainers[i_width + i_height] = write_color(pixel_color, i_height, i_width, samples_per_pixel);
//                colors.at(i_width+i_height) = write_color(pixel_color, samples_per_pixel);
//                values_push_back(write_color(pixel_color, i_height, i_width, samples_per_pixel));
                colorsContainers.push_back(write_color(pixel_color, i_height, i_width, samples_per_pixel));
//                colors.push_back(write_color(pixel_color, samples_per_pixel));

            }
        }
//    }
    std::cout << "Thread ID : " << sizeof(colorsContainers) <<std::endl ;
    savebmp("dummy.bmp", image_width, image_height, 1, colorsContainers);
    t2 = clock();
    float diff = ((float)t2 - (float)t1)/1000;

    std::cout << diff << " seconds" << std::endl;
    std::cerr << "\nDone.\n";
}
