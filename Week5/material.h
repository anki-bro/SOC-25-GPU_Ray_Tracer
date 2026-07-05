#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material {
    public:
        virtual ~material() = default; 

        virtual color3 emitted(double u, double v, const point3& p) const {
            return color3(0, 0, 0); // default implementation returns black (no emission)
        }
        virtual bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const {
            return false; // default implementation does not scatter
        }
};

class lambertian : public material {
    public:
        lambertian(shared_ptr<texture> a) : albedo_texture(a) {}

        lambertian(const color3& a) : albedo_texture(make_shared<solid_color>(a)) {}

        bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + Vec3::random_unit_vector();
            if(scatter_direction.near_zero()) {
                scatter_direction = rec.normal; // if random direction is near zero, use the normal
            }
            scattered = ray(rec.p, scatter_direction,r_in.time());
            attenuation = albedo_texture->value(rec.u, rec.v, rec.p); // use texture for attenuation
            return true;
        }
    private:
        shared_ptr<texture> albedo_texture; // texture for the material's color
};

class metal : public material {
    public:
        metal(const color3& a, float fuzz) : albedo(a), fuzz(fuzz < 1 ? fuzz:1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const override {
            Vec3 reflected = r_in.getDirection().reflect(rec.normal);
            reflected = reflected.unit() + Vec3::random_unit_vector() * fuzz; // add fuzziness
            scattered = ray(rec.p, reflected, r_in.time());
            attenuation = albedo;
            return (scattered.getDirection().dot(rec.normal)>0); // Metal always scatters
        }
    private:
        color3 albedo; // Reflective color of the material
        float fuzz;   // Fuzziness factor for the metal
};

class dielectric : public material {
    public:
        dielectric(float ri) : refractive_index(ri) {}

        bool scatter(const ray& r_in, const hit_record& rec, color3& attenuation, ray& scattered) const override {
            attenuation = color3(1.0, 1.0, 1.0); // Dielectric materials do not change color
            float refraction_ratio = rec.front_face ? (1.0 / refractive_index) : refractive_index;

            Vec3 unit_direction = r_in.getDirection().unit();
            float cos_theta = std::fmin(-unit_direction.dot(rec.normal), 1.0);
            float sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
            Vec3 directed;
            if((refraction_ratio * sin_theta > 1.0) || reflectance(cos_theta, refraction_ratio)>random_double()) {  // tir
                directed = unit_direction.reflect(rec.normal);
            }
            else {
                directed = unit_direction.refract(rec.normal, refraction_ratio);
            }
            scattered = ray(rec.p, directed, r_in.time());
            return true;
        }

    private:
        float refractive_index; // Refractive index of the dielectric material
        static float reflectance(float cosine, float ri) {
            // Schlick's approximation for reflectance
            float r0 = (1 - ri) / (1 + ri);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cosine), 5);
        }
};

class diffuse_light : public material {
    public:
        diffuse_light(shared_ptr<texture> texture) : emit_texture(texture) {}
        diffuse_light(const color3& emit) : emit_texture(make_shared<solid_color>(emit)) {}

        color3 emitted(double u, double v, const point3& p) const {
            return emit_texture->value(u, v, p); // Return the emitted color from the texture
        }
    private:
        shared_ptr<texture> emit_texture; // Texture for the emitted light
};

#endif