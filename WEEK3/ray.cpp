#include "config.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

color3 ray_color(const ray& r, const hittable_list& world) {
    hit_record rec;
    if (world.hit(r, interval(0.0, infinity), rec)) {
        return color3(0.5 * (rec.normal.x() + 1.0), 0.5 * (rec.normal.y() + 1.0), 0.5 * (rec.normal.z() + 1.0));
    }
    Vec3 unit_direction = r.getDirection().unit();
    auto a = 0.5*(unit_direction.y() + 1.0);
    return color3(1.0, 1.0, 1.0)*(1.0-a) + color3(0.5, 0.7, 1.0)*a;
}

int main() {

    float aspect_ratio = 16.0 / 10.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height<1) ? 1 : image_height; 

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    float focal_length = 1.0;
    point3 camera_center(0, 0, 0);
    float viewport_height = 2.0;
    float viewport_width = viewport_height* float((image_width) /(image_height));

    Vec3 viewport_u = Vec3(viewport_width, 0, 0);
    Vec3 viewport_v = Vec3(0, -viewport_height, 0);

    Vec3 delta_u = viewport_u / (image_width);
    Vec3 delta_v = viewport_v / (image_height);

    point3 upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2 ;
    point3 pixel00_location = upper_left + (delta_u  + delta_v )*0.5;


    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_location + (delta_u * i) + (delta_v * j);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color3 pixel_color = ray_color(r,world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}