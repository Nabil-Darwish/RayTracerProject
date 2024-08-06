#ifndef SHAPE_H
#define SHAPE_H

#include "math_utils.h"

class Material; // Forward declaration
class Scene;

class Hit_record { // Record of the hit
    public:
        float t;                            // t scalar of the ray
        Vector3D p;                         // Point of intersection
        Vector3D normal;                    // Normal at the point of intersection
        std::shared_ptr<Material> mat_ptr;  // Material of the object
        bool front_face;                    // Is the ray hitting the front face of the object?

        void set_face_normal(const Ray& r, const Vector3D& outward_normal) {
            front_face = dotProduct(r.getDirection(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class Shape {
    protected:
        Scene* scene; // Pointer to the scene the shape is in
        
    public:
        virtual ~Shape() {}

        virtual bool hit(const Ray& r, Interval ray_t, Hit_record& rec, const Point3D* check_point = nullptr) const = 0;

        void setScene(Scene* _scene) {scene = _scene;}

        virtual bool is_light_source_obstructed(const Ray& ray, Interval ray_t, const Point3D& light_position) const = 0;

        virtual std::shared_ptr<Material> getMaterial() const = 0;

        virtual Point3D getCenter() const = 0;
        virtual double getRadius() const = 0;
        virtual double getHeight() const = 0;

        virtual bool is_sphere() const {return false;}
        virtual bool is_triangle() const {return false;}
        virtual bool is_cylinder() const {return false;}

        virtual void print() const = 0;
};

#endif