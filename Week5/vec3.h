#ifndef VEC3_H
#define VEC3_H

class Vec3 {
public:
    float v[3];

    Vec3(float x = 0, float y = 0, float z = 0) : v{x, y, z} {}

    float x() const { 
        return v[0]; 
    }
    float y() const { 
        return v[1]; 
    }   
    float z() const { 
        return v[2]; 
    }

    Vec3 operator+(const Vec3& other) const { 
        return Vec3(v[0] + other.x(), v[1] + other.y(), v[2] + other.z()); 
    }
    Vec3 operator-(const Vec3& other) const { 
        return Vec3(v[0] - other.x(), v[1] - other.y(), v[2] - other.z()); 
    }
    Vec3 operator*(const float scalar) const { 
        return Vec3(v[0] * scalar, v[1] * scalar, v[2] * scalar); 
    }
    Vec3 operator*(const Vec3& other) const { 
        return Vec3(v[0] * other.x(), v[1] * other.y(), v[2] * other.z()); 
    }
    Vec3 operator/(const float scalar) const { 
        if (scalar == 0) throw std::runtime_error("Division by zero");
        return Vec3(v[0] / scalar, v[1] / scalar, v[2] / scalar);
    }
    Vec3 operator-() const { 
        return Vec3(-v[0], -v[1], -v[2]); 
    }
    float& operator[](const int i) {
        if (-1<i<3) {
            return v[i];
        } else {
            throw std::out_of_range("Index must be 0, 1, or 2");
        }
    }
    float length() const {
        return std::sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    }

    float length_squared() const {
        return (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
    }

    bool near_zero() const {
        return ((std::fabs(v[0]) < 1e-8) && (std::fabs(v[1]) < 1e-8) &&(std::fabs(v[2]) < 1e-8));
    }
    
    static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    static Vec3 random(float min, float max) {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    inline Vec3 unit() const {
        float len = length();
        if (len == 0) {
            throw std::runtime_error("Zero-length vector");
        }
        return *this / len;
    }

    static Vec3 random_in_unit_disk() {
        while (true) {
            Vec3 p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
            if (p.length_squared() < 1.0) {
                return p; 
            }
        }
    }
    
    static Vec3 random_unit_vector() {
        while (true) {
            Vec3 p = Vec3::random(-1, 1);
            auto len_squared = p.length_squared();
            if (1e-8 < len_squared && len_squared <= 1.0) {
                return p; 
            }
        }
    }

    inline Vec3 random_on_hemisphere(const Vec3& normal) {
        Vec3 in_unit_sphere = random_unit_vector();
        if (in_unit_sphere.dot(normal) > 0.0f) {
            return in_unit_sphere;
        } else {
            return -in_unit_sphere;
        }
    }

    float dot(const Vec3& other) const {
        return (v[0] * other.x() + v[1] * other.y()+ v[2] * other.z());
    }

    Vec3 cross(const Vec3& other) const {
        return Vec3(
            v[1] * other.z() - v[2] * other.y(),
            v[2] * other.x() - v[0] * other.z(),
            v[0] * other.y() - v[1] * other.x());
    }

    Vec3 reflect(const Vec3& normal) const {
        return *this - (normal.unit() * (2.0f * this->dot(normal.unit())));
    }

    Vec3 refract(const Vec3& normal, float refractive_index) const {
        Vec3 u = this->unit();
        Vec3 n = normal.unit();
        float cos = std::min(-u.dot(n), 1.0f);
        Vec3 r_perp = (u + n * cos) * refractive_index;
        float k = 1.0f - (r_perp.length() * r_perp.length());
        if (k < 0) {
            return Vec3(0, 0, 0); // Total internal reflection
        }
        return r_perp - n * std::sqrt(k);
    }
};
using point3 = Vec3;
using color3 = Vec3;
#endif // VEC3_H