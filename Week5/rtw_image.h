#ifndef RTW_IMAGE_H
#define RTW_IMAGE_H

#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

class rtw_image {
public:
    rtw_image() {}

    rtw_image(const char *filename) { 
        load(std::string(filename)); 
    }

    ~rtw_image() { 
        delete[] bdata; 
        if (fdata) {
            stbi_image_free(fdata); 
        }
    }

    bool load(const std::string &filename) {
        int n = 0;
        fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
        if (!fdata) return false;
        bytes_per_scanline = image_width * bytes_per_pixel;

        int total = image_width * image_height * bytes_per_pixel;
        bdata = new unsigned char[total];
        for (int i = 0; i < total; ++i) bdata[i] = float_to_byte(fdata[i]);
        return true;
    }

    int width() const { return image_width; }
    int height() const { return image_height; }

    const unsigned char *pixel_data(int x, int y) const {
        static unsigned char magenta[3] = { 255, 0, 255 };
        if (!bdata) return magenta;

        if (x < 0) x = 0; if (y < 0) y = 0;
        if (x >= image_width) x = image_width - 1;
        if (y >= image_height) y = image_height - 1;

        return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }

    const float *float_data() const { return fdata; }

private:
    const int bytes_per_pixel = 3;
    float *fdata = nullptr;
    unsigned char *bdata = nullptr;
    int image_width = 0;
    int image_height = 0;
    int bytes_per_scanline = 0;

    static unsigned char float_to_byte(float v) {
        if (v <= 0.0f) return 0;
        if (v >= 1.0f) return 255;
        return static_cast<unsigned char>(v * 255.0f);
    }
};

#endif // RTW_IMAGE_H
