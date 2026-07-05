#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <atomic>
#include <chrono>
#include <pthread.h>
#include <unistd.h>
#include <vector>

class camera {
    public:
        float aspect_ratio = 1.0;
        int image_width = 100;
        int sample_per_pixel = 10;
        int max_depth = 10;
        color3 background = color3(0, 0, 0);
        float vfov = 90;
        point3 lookfrom = point3(0, 0, 0);
        point3 lookat = point3(0, 0, -1);
        Vec3 vup = Vec3(0, 1, 0);
        float defocus_angle = 0.0; 
        float focus_distance = 10.0;
        
        void render(const hittable& world) {
            auto start = std::chrono::high_resolution_clock::now();
            initialise();

            std::vector<color3> framebuffer(image_width * image_height);
            std::atomic<int> next_row(0);
            std::atomic<int> rows_done(0);

            int thread_count = static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN));
            if (thread_count < 1) {
                thread_count = 4;
            }

            render_task task{this, &world, &framebuffer, &next_row, &rows_done};

            std::vector<pthread_t> threads(thread_count);

            for (int t = 0; t < thread_count; t++) {
                pthread_create(&threads[t], nullptr, render_worker, &task);
            }

            while (rows_done.load() < image_height) {
                std::clog << "\rScanlines remaining: " << (image_height - rows_done.load()) << ' ' << std::flush;
                usleep(100000);
            }

            for (int t = 0; t < thread_count; t++) {
                pthread_join(threads[t], nullptr);
            }

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int j = 0; j < image_height; j++) {
                for (int i = 0; i < image_width; i++) {
                    write_color(std::cout, framebuffer[j * image_width + i]);
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

        struct render_task {
            const camera* cam;
            const hittable* world;
            std::vector<color3>* framebuffer;
            std::atomic<int>* next_row;
            std::atomic<int>* rows_done;
        };

        static void* render_worker(void* arg) {
            auto* task = static_cast<render_task*>(arg);

            while (true) {
                int j = task->next_row->fetch_add(1);
                if (j >= task->cam->image_height) {
                    break;
                }

                for (int i = 0; i < task->cam->image_width; i++) {
                    (*task->framebuffer)[j * task->cam->image_width + i] =
                        task->cam->render_pixel(i, j, *task->world);
                }
                task->rows_done->fetch_add(1);
            }

            return nullptr;
        }

        color3 render_pixel(int i, int j, const hittable& world) const {
            color3 pixel_color(0, 0, 0);
            for (int s = 0; s < sample_per_pixel; s++) {
                ray r = get_ray(i,j);
                pixel_color = pixel_color + ray_color(r, max_depth, world);
            }
            return pixel_color * pixel_size;
        }

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
            auto time = random_double(0.0, 1.0);
            return ray(ray_origin, ray_direction, time);
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

            if(!world.hit(r, interval(0.001, infinity), rec)) {
                return background; // Return background color if no hit occurs
            }

            color3 attenuation;
            ray scattered;
            color3 emitted_color = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

                // If the material scatters, return emitted + attenuated scattered color
                if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                    return emitted_color + ray_color(scattered, depth-1, world) * attenuation;
                }

                // Otherwise (e.g., light sources), return only the emitted color
                return emitted_color;
            
            Vec3 unit_direction = r.getDirection().unit();
            auto a = 0.5*(unit_direction.y() + 1.0);
            return color3(1.0, 1.0, 1.0)*(1.0-a) + color3(0.5, 0.7, 1.0)*a;
        }
};
#endif // CAMERA_H
