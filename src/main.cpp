#include "common.h"

#include "camera.h"
#include "material.h"
#include "volumes/bvh.h"
#include "volumes/sphere.h"
#include "volumes/volume.h"
#include "volumes/volume_list.h"

void bouncing_spheres() {
    volume_list world;

    auto checker = make_shared<checker_texture>(color(.2, .3, .1), color(.9, .9, .9), 0.32);
    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
            point3 center2 = center;

            if ((center - point3(4, 0.2, 0)).length_squared() > 0.81) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    center2 += point3(0, random_double(0,0.5), 0);
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                } else {
                    sphere_material = make_shared<dielectric>(1.5);
                }

                world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = volume_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;
    cam.max_depth = 10;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void checkered_spheres() {
    volume_list world;

    auto checker = make_shared<checker_texture>(color(.2, .3, .1), color(.9, .9, .9), 0.32);

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(volume_list(globe));
}

void perlin_spheres() {
    volume_list world;

    auto pertext = make_shared<noise_texture>(4, 10, 7);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}


int main() {
    perlin_spheres();
}