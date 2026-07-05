#include "config.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

void bouncing_spheres() {
    hittable_list world;

    // Materials with brighter colors for better visibility
    auto checker = make_shared<checker_texture>(0.32, color3(0.8, 0.8, 0.8), color3(0.2, 0.2, 0.2));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    auto center2 = center + Vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    auto center2 = center + Vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.sample_per_pixel = 20;
    cam.max_depth         = 20;
    cam.background        = color3(0.70, 0.80, 1.00);
    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = Vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_distance = 10.0;

    cam.render(world);
}

void checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.1, color3(1, 1, 1), color3(0, 0, 0));
    world.add(make_shared<sphere>(point3(0,-12,0), 12, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 12, 0), 12, make_shared<lambertian>(checker)));

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.sample_per_pixel = 20;
    cam.max_depth         = 20;
    cam.background        = color3(0.70, 0.80, 1.00);
    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = Vec3(0,1,0);
    cam.defocus_angle = 0.6;
    cam.render(world);
}

void earth(){
    hittable_list world;

    auto earth_texture = make_shared<image_texture>("earth_map.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    world.add(make_shared<sphere>(point3(0,0,0), 2, earth_surface));

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.sample_per_pixel = 100;
    cam.max_depth         = 20;
    cam.background        = color3(0.70, 0.80, 1.00);
    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = Vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void perlin_spheres() {
    hittable_list world;

    auto perlin_texture = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(perlin_texture)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(perlin_texture)));

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.sample_per_pixel = 100;
    cam.max_depth         = 20;
    cam.background        = color3(0.70, 0.80, 1.00);
    cam.vfov     = 20;
    cam.lookfrom = point3(14,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = Vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void simple_light() {
    hittable_list world;

    auto perlin_texture = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(perlin_texture)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(perlin_texture)));

    auto difflight = make_shared<diffuse_light>(color3(4,4,4));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.sample_per_pixel = 400;
    cam.max_depth         = 50;
    // cam.background        = color3(0.70, 0.80, 1.00);
    cam.background        = color3(0, 0, 0); // black background for better contrast with light
    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = Vec3(0,1,0);
    cam.defocus_angle = 0;
    cam.render(world);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    // hittable_list boxes1;
    // auto ground = make_shared<lambertian>(color3(0.48, 0.83, 0.53));

    // int boxes_per_side = 20;
    // for (int i = 0; i < boxes_per_side; i++) {
    //     for (int j = 0; j < boxes_per_side; j++) {
    //         auto w = 100.0;
    //         auto x0 = -1000.0 + i*w;
    //         auto z0 = -1000.0 + j*w;
    //         auto y0 = 0.0;
    //         auto x1 = x0 + w;
    //         auto y1 = random_double(1,101);
    //         auto z1 = z0 + w;

    //         boxes1.add(box(point3(x0,y0,z0), point3(x1,y1,z1), ground));
    //     }
    // }

    hittable_list world;

    // world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<diffuse_light>(color3(7, 7, 7));
    world.add(make_shared<sphere>(point3(123,554,147), 300, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + Vec3(30,0,0);
    auto sphere_material = make_shared<lambertian>(color3(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color3(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    // world.add(make_shared<constant_medium>(boundary, 0.2, color3(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    // world.add(make_shared<constant_medium>(boundary, .0001, color3(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("earth_map.jpg"));
    world.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.2);
    world.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color3(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    // world.add(make_shared<translate>(
    //     make_shared<rotate_y>(
    //         make_shared<bvh_node>(boxes2), 15),
    //         vec3(-100,270,395)
    //     )
    // );

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = image_width;
    cam.sample_per_pixel = samples_per_pixel;
    cam.max_depth         = max_depth;
    cam.background        = color3(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = Vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    // bouncing_spheres();
    // checkered_spheres();
    // earth();
    // perlin_spheres();
    // simple_light();
    final_scene(800, 1000, 100);
    return 0;
}
