#ifndef RENDER_H
#define RENDER_H

#include <sycl/sycl.hpp>
#include <vector>

#include "vec3.h"
#include "config.h"

template <int width, int height, int num_spheres = 0>
void render(sycl::queue &queue, Vec3 *framebuffer, const sphere *spheres){

    constexpr auto num_pixels = width * height;
    auto frame_buffer=sycl::buffer<Vec3,1>(frame_buffer, sycl::range<1>(num_pixels));
    auto spheres_buffer = sycl::buffer<sphere, 1>(spheres, sycl::range<1>(num_spheres));
    
    queue.submit([&](sycl::handler &cgh) {
        auto frame_pointer = frame_buffer.get_access<sycl::access::mode::write>(cgh);
        auto spheres_pointer = spheres_buffer.get_access<sycl::access::mode::read>(cgh);
        
        const auto global = sycl::range<2>(width, height);
        const auto local = sycl::range<2>(constants::TileX, constants::TileY);
        const auto nd_range = sycl::nd_range<2>(global, local);
        auto render_kernel = render_kernel<width, height, num_spheres>(frame_pointer, sphere_pointer);
        cgh.parallel_for(nd_range, render_kernel);
    });
}