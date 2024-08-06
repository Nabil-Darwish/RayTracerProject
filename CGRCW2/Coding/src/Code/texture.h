#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "math_utils.h"

#include <fstream>
#include <iostream>

#include "stb_image.h"

class Texture {
public:
    virtual ~Texture() = default;
    virtual Color value(double u, double v, const Vector3D& p) const = 0;
};

class ImageTexture : public Texture {
public:
    ImageTexture::ImageTexture(const std::string& filename) {
    // Number of color channels in the image
    int channels;

    // Load the image file using stb_image
    // stbi_load returns a pointer to the image data and fills in the width, height, and channels
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    // Check if the image was loaded successfully
    if (!img) {
        // If the image could not be loaded, print an error message and set width and height to 0
        std::cerr << "Error: Could not load texture image file '" << filename << "'." << std::endl;
        width = height = 0;
    } else {
        // If the image was loaded successfully, allocate an array to hold the color data
        data = new Color[width * height];

        // Loop over each pixel in the image
        for (int i = 0; i < width * height; ++i) {
            // Get the color components of the pixel
            // The color data is stored in the image data as unsigned chars, with each color component in a separate channel
            auto r = img[channels * i];
            auto g = img[channels * i + 1];
            auto b = img[channels * i + 2];

            // Convert the color components from the range [0, 255] to [0, 1] and store them in the color data array
            data[i] = Color(r, g, b) / 255.0;
        }

        // Free the image data returned by stbi_load
        stbi_image_free(img);
    }
}

virtual Color value(double u, double v, const Vector3D& p) const override;

private:
    // Add data members to store the image data
    int width, height;
    Color* data;

    // Add helper functions to read in the image data
    void get_sphere_uv(const Vector3D& p, double& u, double& v) {
        auto theta = acos(-p.y);
        auto phi = atan2(-p.z, p.x) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};

#endif