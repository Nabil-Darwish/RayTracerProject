#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"
#include "material.h"

class Scene;

class Sphere : public Shape
{
    public:
        // Default constructor
        Sphere(Point3D _center, double _radius, std::shared_ptr<Material> _material) : center(_center), radius(_radius), mat(_material) {}

        // Check if ray intersects sphere, update the hit record if it does
        // Alternatively, if a check_point is passed, check if the closest intersection point is the same as *check_point. If it is, return true, else return false. No update to the hit record is made.
        virtual bool hit(const Ray& ray, Interval ray_t, Hit_record& record, const Point3D* check_point) const override{
            Vector3D oc = ray.getOrigin() - center;
            double a = getLengthSquared(ray.getDirection());
            double half_b = dotProduct(oc, ray.getDirection());
            double c = getLengthSquared(oc) - radius * radius;
            double discriminant = half_b * half_b - a * c;

            if (discriminant < 0) return false;
            double sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            double root = (-half_b - sqrtd) / a;
            // If the root is not in the acceptable range, try the other root
            if (!(ray_t.contains(root))) {
                // If the other root is not in the acceptable range, return false
                root = (-half_b + sqrtd) / a;
                if (!(ray_t.contains(root)))
                    return false;
            }

            // If past this point, intersection point found and is in acceptable range

            // Calculate the intersection point
            auto t = root;
            auto position = ray.at(t);

            // If check_point a nullptr, skip this step
            // If not, shadow ray is being traced. Check if the intersection point is the same as the check_point
            if (check_point != nullptr) {
                // If the intersection point is not the same as the check_point, return true
                if (position != *check_point) {
                    // This means there was some other shape that was closer to the light source than this sphere
                    return true;
                } else {
                    return false;
                }
            }

            record.t = t;
            record.p = position;
            Vector3D outward_normal = (record.p - center) / radius;
            record.set_face_normal(ray, outward_normal);
            record.mat_ptr = mat;

            return true;
        }

        // Checks if a shadow ray from intersection point can be traced back to the light source. If ray is traceable, it shouldn't hit any other shapes in the scene.
        virtual bool is_light_source_obstructed(const Ray& ray, Interval ray_t, const Point3D& light_position) const {
            Hit_record temp_rec;
            // Check if shadow ray intersects any of the shapes in the scene
            if (hit(ray, ray_t, temp_rec, &light_position)) {
                return true;
            } else {
                return false;
            }
        }

        virtual Point3D getCenter() const override {return center;}
        virtual double getRadius() const override {return radius;}
        virtual std::shared_ptr<Material> getMaterial() const override {return mat;}
        virtual double getHeight() const override {return 0;}

        virtual bool is_sphere() const override {return true;}
        virtual bool is_triangle() const override {return false;}
        virtual bool is_cylinder() const override {return false;}

        virtual void print() const override{
            std::clog << "Sphere:\n"
                    << "Center: (" << center.x << ", " << center.y << ", " << center.z << ")\n"
                    << "Radius: " << radius << "\n";
        }

    private:
        Point3D center;
        double radius;
        std::shared_ptr<Material> mat;
};

#endif