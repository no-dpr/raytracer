#include "common.h"
#include "camera.h"
#include "volumes/volume.h"
#include "volumes/volume_list.h"
#include "volumes/sphere.h"

int main() {
    volume_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;

    cam.render(world);
}