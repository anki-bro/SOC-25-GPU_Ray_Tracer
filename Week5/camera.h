#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <chrono>

class camera {
    public:
        float aspect_ratio = 1.0;
        int image_width = 100;
        int sample_per_pixel = 10;
        int max_depth = 10;
        float vfov = 90;
        point3 lookfrom = point3(0, 0, 0);
        point3 lookat = point3(0, 0, -1);
        Vec3 vup = Vec3(0, 1, 0);
        float defocus_angle = 0.0; 
        float focus_distance = 10.0;
        
        void render(const hittable& world) {
            auto start = std::chrono::high_resolution_clock::now();
            initialise();
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color3 pixel_color(0, 0, 0);
                    for (int s = 0; s < sample_per_pixel; s++) {
                        ray r = get_ray(i,j);
                        pixel_color = pixel_color + ray_color(r, max_depth, world);
                    }
                    write_color(std::cout, pixel_color * pixel_size);
                }
            }
            std::clog << "\rDone.                 \n";
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::clog << "Time taken: " << double(duration.count()/1000.000) << " s\n";
        }
    private:
        int image_height;
        point3 center;
        double pixel_size;
        point3 pixel00_location;
        Vec3 delta_u;
        Vec3 delta_v;
        Vec3 u, v, w;
        Vec3 defocus_disk_u;
        Vec3 defocus_disk_v;

        void initialise() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height; 
            pixel_size = 1.0 / sample_per_pixel;

            center=lookfrom;
            auto theta = degrees_to_radians(vfov);
            float half_height = std::tan(theta / 2);
            float viewport_height = 2.0 * half_height * focus_distance;
            float viewport_width = viewport_height * (double(image_width)/image_height);

            w = (lookfrom - lookat).unit();
            u = (vup.cross(w)).unit();
            v = w.cross(u);

            Vec3 viewport_u = u * viewport_width;
            Vec3 viewport_v = -v * viewport_height;

            delta_u = viewport_u / image_width;
            delta_v = viewport_v / image_height;

            point3 upper_left = center - (w * focus_distance) - viewport_u / 2 - viewport_v / 2 ;
            pixel00_location = upper_left + (delta_u  + delta_v )*0.5;

            auto defocus_disk_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_disk_radius;
            defocus_disk_v = v * defocus_disk_radius;
        }

        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = pixel00_location + (delta_u*(i+offset.x())) + (delta_v*(j+offset.y()));
            auto ray_origin = (defocus_angle<=0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;
            return ray(ray_origin, ray_direction);
        }

        Vec3 sample_square() const {
            return Vec3(random_double()-0.5, random_double()-0.5, 0);
        }

        point3 defocus_disk_sample() const {
            auto disk_sample = Vec3::random_in_unit_disk();
            return center + defocus_disk_u * disk_sample[0] + defocus_disk_v * disk_sample[1];
        }

        color3 ray_color(const ray& r, int depth, const hittable& world) const {
            if(depth <= 0) {
                return color3(0, 0, 0); 
            }
            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                color3 attenuation;
                ray scattered;
                if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                    auto scattered_color = ray_color(scattered, depth-1, world);
                    return scattered_color * attenuation; // Return the color after scattering
                }
                return color3(0, 0, 0); // Return black if no scattering occurs
            }
            Vec3 unit_direction = r.getDirection().unit();
            auto a = 0.5*(unit_direction.y() + 1.0);
            return color3(1.0, 1.0, 1.0)*(1.0-a) + color3(0.5, 0.7, 1.0)*a;
        }
};
#endif // CAMERA_H