#ifndef VECTOR_OPERATIONS_H
#define VECTOR_OPERATIONS_H

#include "interval.h"

#include <cmath>
#include <iostream>


struct Vector3D {
    double x;
    double y;
    double z;

    // Default constructor
    Vector3D() : x(0), y(0), z(0) {}

    // Parameterized constructor
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    // Addition assignment operator
    Vector3D& operator+=(const Vector3D& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    // Subtraction assignment operator
    Vector3D& operator-=(const Vector3D& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    // Multiplication assignment operator
    Vector3D& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Division assignment operator
    Vector3D& operator/=(double scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }


    static Vector3D random() {
        return Vector3D(random_double(), random_double(), random_double());
    }

    static Vector3D random(double min, double max) {
        return Vector3D(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    friend bool operator!=(const Vector3D& v1, const Vector3D& v2);
};

// Negation of a vector
Vector3D operator-(const Vector3D& v) {
    return Vector3D(-v.x, -v.y, -v.z);
}

// Equivalence operator
bool operator==(const Vector3D& v1, const Vector3D& v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

// Non-equivalence operator
bool operator!=(const Vector3D& v1, const Vector3D& v2) {
    return !(v1 == v2);
}

// Addition of two vectors
Vector3D operator+(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// Subtraction of two vectors
Vector3D operator-(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// Cross multiplication of two vectors
Vector3D crossProduct(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

// Dot multiplication of two vectors
double dotProduct(const Vector3D& v1, const Vector3D& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Element-wise multiplication of two vectors
Vector3D operator*(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

// Scalar multiplication of a vector
Vector3D operator*(const Vector3D& v, double scalar) {
    return Vector3D(v.x * scalar, v.y * scalar, v.z * scalar);
}

// Scalar multiplication of a vector but with the scalar on the left
Vector3D operator*(double scalar, const Vector3D& v) {
    return Vector3D(v.x * scalar, v.y * scalar, v.z * scalar);
}

// Scalar division of a vector
Vector3D operator/(const Vector3D& v, double scalar) {
    return Vector3D(v.x / scalar, v.y / scalar, v.z / scalar);
}

// Hadamard division of two vectors
Vector3D operator/(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// Get the length squared of a vector
double getLengthSquared(const Vector3D& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
};

bool near_zero(const Vector3D& v) {
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s);
}

Vector3D clampVector(const Vector3D& v) {
    Interval unit = Interval(0,1);
    return Vector3D(unit.clamp(v.x), unit.clamp(v.y), unit.clamp(v.z));
}

// Get the length of a vector
double getLength(const Vector3D& v) {
    return sqrt(getLengthSquared(v));
};

// Normalize a vector
Vector3D normalize(const Vector3D& v) {
    return v / getLength(v);
};

// Get a random vector in unit sphere
Vector3D random_in_unit_sphere() {
    while (true) {
        auto p = Vector3D::random(-1, 1);
        if (getLengthSquared(p) >= 1) continue;
        return p;
    }
}

// Get a random unit vector
Vector3D random_unit_vector() {
    return normalize(random_in_unit_sphere());
}

// Get a random vector in unit disk
Vector3D random_in_hemisphere(const Vector3D& normal) {
    Vector3D in_unit_sphere = random_unit_vector();
    if (dotProduct(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vector3D pixel_sample_disk(const Vector3D u, const Vector3D v) {
    // Returns random point in the pixel disk
    double theta = 2 * pi * random_double();  // Random angle
    double r = sqrt(random_double());  // Random radius
    auto px = r * cos(theta);
    auto py = r * sin(theta);
    return (px * u) + (py * v);
}

Vector3D pixel_sample_square(const Vector3D u, const Vector3D v) {
    // Returns random point in the pixel square
    auto px = -0.5 + random_double();
    auto py = -0.5 + random_double();
    return (px * u) + (py * v);
}

// Reflect a vector
Vector3D reflect(const Vector3D& v, const Vector3D& n) {
    return v - 2 * dotProduct(v, n) * n;
}

// Refract a vector
Vector3D refract(const Vector3D& uv, const Vector3D& n, double etai_over_etat) {
    auto cos_theta = fmin(dotProduct(-uv, n), 1.0);
    Vector3D r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3D r_out_parallel = -sqrt(fabs(1.0 - getLengthSquared(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}

// Allows us to print a vector
std::ostream& operator<<(std::ostream& os, const Vector3D& c) {
    os << "Color(" << c.x << ", " << c.y << ", " << c.z << ")";
    return os;
}

// Point3D is alias for Vector3D
using Point3D = Vector3D;

#endif // VECTOR_OPERATIONS_H
