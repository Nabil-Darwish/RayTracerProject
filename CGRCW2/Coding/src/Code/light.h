#ifndef LIGHT_H
#define LIGHT_H

#include "math_utils.h"
#include "color.h"

// Light base class
class Light {
public:
    virtual ~Light() = default;
    virtual Vector3D getPosition() const { return Vector3D(); }
    virtual Color getIntensity() const { return Color(); }

};

// PointLight class
class PointLight : public Light {
public:
    PointLight(const Point3D& position, const Color& intensity)
        : position(position), intensity(intensity) {}

    Point3D getPosition() const { return position; }
    Color getIntensity() const { return intensity; }

private:
    Point3D position; // Position of the light
    Color intensity;   // Intensity of the light
};

// AreaLight class
class AreaLight : public Light {
public:
    AreaLight(/* parameters */) {
        // Initialization code
    }

private:
    // Data members for the area light
    // These will depend on how you want to implement area lights
};

#endif // LIGHT_H