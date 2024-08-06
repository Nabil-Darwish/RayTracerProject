#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include <algorithm>

class Triangle : public Shape
{
    public:
        Triangle(Point3D v0, Point3D v1, Point3D v2, std::shared_ptr<Material> _material) : v0(v0), v1(v1), v2(v2), mat(_material) {};
        //Triangle(Point3D v0, Point3D v1, Point3D v2) : v0(v0), v1(v1), v2(v2) {};

        virtual bool hit(const Ray& ray, Interval ray_t, Hit_record& record, const Point3D* check_point) const override {
            Vector3D e1 = v1 - v0;
            Vector3D e2 = v2 - v0;
            Vector3D p = crossProduct(ray.getDirection(), e2);
            double a = dotProduct(e1, p);
            Interval epsilon(-0.00001, 0.00001);
            if (epsilon.surrounds(a)) {
                return false;
            }
            double f = 1.0 / a;
            Vector3D s = ray.getOrigin() - v0;
            double u = f * dotProduct(s, p);
            Interval uInterval(0.0, 1.0);
            if (!uInterval.contains(u)) {
                return false;
            }
            Vector3D q = crossProduct(s, e1);
            double v = f * dotProduct(ray.getDirection(), q);
            Interval vInterval(0.0, 1.0 - u);
            if (!vInterval.contains(v)) {
                return false;
            }
            double t = f * dotProduct(e2, q);
            if (!ray_t.contains(t)) {
                return false;
            }
            auto position = ray.at(t);
            if (check_point != nullptr) {
                if (position != *check_point) {
                    return false;
                } else {
                    return true;
                }
            }
            record.t = t;
            record.p = position;
            record.mat_ptr = mat;
            Vector3D outward_normal = crossProduct(e1, e2);
            if (dotProduct(ray.getDirection(), outward_normal) < 0) {
                // Ray is hitting the front face of the triangle
                outward_normal = normalize(outward_normal);
                record.set_face_normal(ray, outward_normal);        //For some reason, still giving same normal. Check later. Does actually work, just not showing in image
            } else {
                // Ray is hitting the back face of the triangle
                outward_normal = normalize(outward_normal);
                record.set_face_normal(ray, -outward_normal);
            }
            return true;
        }

        virtual bool is_light_source_obstructed(const Ray& ray, Interval ray_t, const Point3D& light_position) const override {
            Hit_record temp_rec;
            // Check if shadow ray intersects any of the shapes in the scene
            if (hit(ray, ray_t, temp_rec, &light_position)) {
                return true;
            } else {
                return false;
            }
        }

        virtual Point3D getCenter() const override {
            return (v0 + v1 + v2) / 3;
        }

        Vector3D get_v0() const {return v0;}
        Vector3D get_v1() const {return v1;}
        Vector3D get_v2() const {return v2;}

        virtual double getRadius() const override {return -1;}
        virtual std::shared_ptr<Material> getMaterial() const override {return mat;}
        virtual double getHeight() const override {return -1;}

        virtual bool is_sphere() const {return false;}
        virtual bool is_triangle() const {return true;}
        virtual bool is_cylinder() const {return false;}

        virtual void print() const override {
            std::clog << "Triangle: " << std::endl;
            std::clog << "v0: " << v0 << std::endl;
            std::clog << "v1: " << v1 << std::endl;
            std::clog << "v2: " << v2 << std::endl;
        }
    
    private:
        Point3D v0, v1, v2;
        std::shared_ptr<Material> mat;

};

#endif
