#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtw_image.h"
#include "perlin.h"

class texture {
    public:
        virtual ~texture() {}

        virtual color3 value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color() {}
        
        solid_color(const color3& c) : albedo(c) {}

        color3 value(double u, double v, const point3& p) const override {
            return albedo;
        }
    private:
        color3 albedo;
};

class checker_texture : public texture {
    public:
        checker_texture() {}
        
        checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
            : inv_scale(1.0/scale), even(even), odd(odd) {}

        checker_texture(double scale, const color3& c1, const color3& c2)
            : inv_scale(1.0/scale), even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

        color3 value(double u, double v, const point3& p) const override {
            auto xint = int(std::floor(p.x() * inv_scale));
            auto yint = int(std::floor(p.y() * inv_scale));
            auto zint = int(std::floor(p.z() * inv_scale));
            if ((xint + yint + zint) % 2 == 0) {
                return even->value(u, v, p);
            } 
            else {
                return odd->value(u, v, p);
            }
        }
    private:
        double inv_scale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
};

class image_texture : public texture {
    public:
        image_texture() {}
        
        image_texture(const char *filename) : img(filename) {}

        color3 value(double u, double v, const point3& p) const override {
            if(!img.width() || !img.height()) {
                return color3(0, 1, 1); //cyan for missing texture
            }
            u = interval(0, 1)._clamp(u);
            v = 1.0 - interval(0, 1)._clamp(v);
            auto i = static_cast<int>(u * img.width());
            auto j = static_cast<int>(v * img.height());
            const unsigned char *pixel = img.pixel_data(i, j);
            auto color_scale = 1.0 / 255.0;
            return color3(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }
    private:
        rtw_image img;
};

class noise_texture : public texture {
    public:
        noise_texture(double scale) : scale(scale) {}

        color3 value(double u, double v, const point3& p) const override {
            return color3(0.5, 0.5, 0.5) * (1 + std::sin(scale * perlin_noise.turbulence(p * 7.0) + 10.0 * perlin_noise.noise(p * 0.5)));
        }
    private:
        perlin perlin_noise;
        double scale;
};

#endif // TEXTURE_H