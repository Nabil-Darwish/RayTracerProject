#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray {
public:
    Point3D origin;
    Vector3D direction;

    Ray() {} // Default constructor

    Ray(const Point3D& origin, const Vector3D& direction)
        : origin(origin), direction(direction) {}

    // Getter methods
    Point3D getOrigin() const { return origin; }
    Vector3D getDirection() const { return direction; }

    // Calculate the point on the ray at t
    Point3D at(double t) const {
        return origin + direction * t;
    }
};

#endif // RAY_H