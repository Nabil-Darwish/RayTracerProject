#ifndef COLOR_H
#define COLOR_H

#include "vector.h"
#include <iostream>

using Color = Vector3D;

// Reinhard tone mapping
Color reinhard_tone_mapping(const Color& color) {
    return color / (color + Color(1.0, 1.0, 1.0));
}

// Function to scale the color values from 0-1 to 0-255
void writeColor(std::ostream &out, const Color& color, int samples_per_pixel) {
    // Write the translated [0,255] value of each color component
    auto r = color.x;
    auto g = color.y;
    auto b = color.z;
    
    // Divide the color by the number of samples
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply Reinhard tone mapping
    Color mapped_color = reinhard_tone_mapping(Color(r, g, b));

    // Gamma-correct for gamma=2.0
    r = sqrt(mapped_color.x);
    g = sqrt(mapped_color.y);
    b = sqrt(mapped_color.z);

    // Write the translated [0,255] value of each color component
    static const Interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << " " 
        << static_cast<int>(256 * intensity.clamp(g)) << " " 
        << static_cast<int>(256 * intensity.clamp(b)) << "\n";
}

#endif // COLOR_H
