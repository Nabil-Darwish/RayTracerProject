#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include <algorithm>

class Cylinder : public Shape
{
    public:
        Cylinder(Point3D _center, Vector3D _axis, double _radius, double _height, std::shared_ptr<Material> _material) : center(_center), axis(_axis), radius(_radius), height(_height), mat(_material) {}
        // Check if ray intersects cylinder, update the hit record if it does
        virtual bool hit(const Ray& ray, Interval ray_t, Hit_record& record, const Point3D* check_point) const override{
            auto epsilon = 0.00001;
            Vector3D oc = ray.getOrigin() - center;
            Vector3D direction = ray.getDirection();
            if (axis == Vector3D(0,0,0)) {
                throw std::invalid_argument("Invalid axis (0,0,0) for Cylinder");
            }
            Vector3D normalized_cylinder_axis = normalize(axis);

            // Project oc and direction onto the normalized cylinder axis
            Vector3D oc_proj = dotProduct(oc, normalized_cylinder_axis) * normalized_cylinder_axis;
            Vector3D direction_proj = dotProduct(direction, normalized_cylinder_axis) * normalized_cylinder_axis;

            // Calculate the discriminant for the intersection with the infinite cylinder
            Vector3D oc_perp = oc - oc_proj;
            Vector3D direction_perp = direction - direction_proj;
            double half_b = dotProduct(oc_perp, direction_perp);
            double a = dotProduct(direction_perp, direction_perp);
            double c = dotProduct(oc_perp, oc_perp) - radius * radius;
            double discriminant = half_b*half_b - a*c;

            // If the discriminant is less than zero, the ray does not intersect the infinite cylinder
            if (discriminant < 0) {
                return false;
            }

            // Calculate the t values for the intersection points with the infinite cylinder
            double sqrt_discriminant = sqrt(discriminant);
            double t1 = (-half_b - sqrt_discriminant) / a;
            double t2 = (-half_b + sqrt_discriminant) / a;

            // Check if the intersection points are within the height of the cylinder
            Vector3D p1 = ray.at(t1);
            Vector3D p2 = ray.at(t2);
            double p1_proj_length = dotProduct(p1 - center, normalized_cylinder_axis);
            double p2_proj_length = dotProduct(p2 - center, normalized_cylinder_axis);

            // Check if ray intersects cylinder caps
            Vector3D cap1_center = center; // bottom cap
            Vector3D cap2_center = center + height * normalized_cylinder_axis; // top cap
            double t3 = dotProduct(cap1_center - ray.getOrigin(), normalized_cylinder_axis) / dotProduct(ray.getDirection(), normalized_cylinder_axis) - epsilon;
            double t4 = dotProduct(cap2_center - ray.getOrigin(), normalized_cylinder_axis) / dotProduct(ray.getDirection(), normalized_cylinder_axis) + epsilon;
            Vector3D p3 = ray.at(t3);
            Vector3D p4 = ray.at(t4);

            double radiusSquared = radius * radius;
            bool intersectsCap1 = t3 >= 0 && getLengthSquared(p3 - cap1_center) <= radiusSquared;
            bool intersectsCap2 = t4 >= 0 && getLengthSquared(p4 - cap2_center) <= radiusSquared;

            bool intersectsBody = (Interval(0, height).contains(p1_proj_length))|| (Interval(0, height).contains(p2_proj_length));

            // Initialize closest valid intersection
            double tmin = std::numeric_limits<double>::max();
            Vector3D normal;
            Vector3D intersection;

            // Check intersections with caps
            if (intersectsCap1) {
                tmin = t3;
                intersection = p3;
            }
            if (intersectsCap2 && t4 < tmin) {
                tmin = t4;
                intersection = p4;
            }

            // Check intersections with body
            if (intersectsBody) {
                if (t1 >= 0 && t1 < tmin && Interval(0, height).contains(p1_proj_length)) {
                    tmin = t1;
                    intersection = p1;
                }
                if (t2 >= 0 && t2 < tmin && Interval(0, height).contains(p2_proj_length)) {
                    tmin = t2;
                    intersection = p2;
                }
            }

            // Calculate normal
            if (tmin == t1 || tmin == t2) {
                Vector3D v = intersection - center;
                Vector3D projection = dotProduct(v, normalized_cylinder_axis) * normalized_cylinder_axis;
                normal = normalize(v - projection);
            } else if (tmin == t3) {
                normal = -normalized_cylinder_axis;
            } else if (tmin == t4) {
                normal = normalized_cylinder_axis;
            }

            // If the ray does not intersect the cylinder, return false
            if (tmin < std::numeric_limits<double>::max()) {
                // If a check_point was provided, check if the closest intersection point is the same as *check_point, otherwise, skip this step
                if (check_point != nullptr) {  
                    // If the intersection point is not the same as the check_point, return true
                    if (intersection != *check_point) {
                        // This means there was some other shape that was closer to the light source than this cylinder
                        return true;
                    } else {
                        return false;
                    }
                }
                // Update the hit record
                record.set_face_normal(ray, normal);
                record.t = tmin;
                record.p = intersection;
                record.mat_ptr = mat;

                return true;
            }

            return false;
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

        virtual Point3D getCenter() const {return center;}
        Vector3D getAxis() const {return axis;}
        virtual double getRadius() const override {return radius;}
        virtual double getHeight() const override {return height;}
        virtual std::shared_ptr<Material> getMaterial() const override {return mat;}

        virtual bool is_sphere() const {return false;}
        virtual bool is_triangle() const {return false;}
        virtual bool is_cylinder() const {return true;}

        virtual void print() const override {
            std::clog << "Cylinder:\n"
                    << "Center: (" << center.x << ", " << center.y << ", " << center.z << ")\n"
                    << "Axis: (" << axis.x << ", " << axis.y << ", " << axis.z << ")\n"
                    << "Radius: " << radius << "\n"
                    << "Height: " << height << "\n";
        }

    private:
        Point3D center;
        Vector3D axis;
        double radius;
        double height;
        std::shared_ptr<Material> mat;
    
};

#endif