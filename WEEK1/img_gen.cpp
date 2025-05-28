#include <iostream>
using namespace std;
int main() {
    int image_width = 256;
    int image_height = 256;
    cout << "P3\n" << image_width << " " << image_height << "\n255" << endl;
    for (int i = 0 ; i < image_height; i++) {
        clog << "\rScanlines remaining: " << image_height - i << ' ' << flush;
        for (int j = 0; j < image_width; j++) {
            auto r = double(i) / (image_width - 1);
            auto g = 3 * double(j) / (image_height - 1); // g is scaled by 0.6
            if (g > 1.0) g = 1.0; // Clamp g to a maximum of 1.0
            if (g < 0.0) g = 0.0; // Clamp g to a minimum of 0.0
            auto b = double(j) / (image_height - 1); // k is either 0, 1, or 2
            int ir = int(256 * r);
            int ig = int(256 * g);
            int ib = int(256 * b);
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    clog << "\nDone.\n";
    return 0;
}