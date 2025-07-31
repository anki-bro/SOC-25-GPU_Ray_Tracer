#ifndef RENDER_KERNEL_H
#define RENDER_KERNEL_H

#include <sycl/sycl.hpp>
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "config.h"
#include "sphere.h"

template <int width, int height, int num_spheres = 0>
class render_kernel {
public:
    render_kernel(sycl::accessor<Vec3, 1, sycl::access::mode::write, sycl::access::target::global_buffer> frame_pointer, sycl::accessor<Sphere, 1, sycl::access::mode::write, sycl::access::target::global_buffer> spheres_pointer) : 
        m_frame_pointer(frame_pointer), m_spheres(spheres_pointer) {}

    void operator()(sycl::nd_item<2> item) const {
        
        int x = item.get_global_id(0);
        int y = item.get_global_id(1);

        const auto pixel_index = y * width + x;
        Vec3 final_color=color();
        m_frame_pointer[pixel_index] = final_color;
    }
private:
        Vec3 color = Vec3(0, 0, 0); // Initialize color to black

        // Ray tracing logic here
        // For simplicity, we will just set a color based on pixel position
        color = Vec3(float(x) / width, float(y) / height, 0.5f);

        framebuffer[y * width + x] = color; // Write the color to the framebuffer
    }