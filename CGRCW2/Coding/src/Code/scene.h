#ifndef SCENE_H
#define SCENE_H

#include "sphere.h"
#include "triangle.h"
#include "cylinder.h"
#include "light.h"

#include <memory>
#include <vector>

// Scene class
// A scene is a collection of shapes
class Sphere;
class Scene : public Shape {
    private: 
        std::vector<std::shared_ptr<Shape>> shapes;
        std::vector<std::shared_ptr<Light>> lights;  // Add this line
    public:

        Scene() {};
        Scene(std::shared_ptr<Shape> shape) {add(shape);}
        Scene(std::shared_ptr<Light> light) {add(light);}

        void clear() {shapes.clear(); lights.clear();}  // Clear lights as well

        void add(std::shared_ptr<Shape> shape) {
            shapes.push_back(shape);
            shape->setScene(this);  // Set the scene of the shape
        }
        void add(std::shared_ptr<Light> light) {lights.push_back(light);}  // Add this function
        // Check if ray intersects scene, update the hit record if it does

        // Getter functions
        std::vector<std::shared_ptr<Shape>> getShapes() const {return shapes;}
        std::vector<std::shared_ptr<Light>> getLights() const {return lights;}  // Add this function
        
        virtual bool hit(const Ray& r, Interval ray_t, Hit_record& rec, const Point3D* check_point = nullptr) const override {
            Hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            // Check if ray intersects any of the shapes in the scene
            // Could be made more efficient by using a bounding volume hierarchy
            for (const auto& shape : shapes) {
                if (shape->hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }

        bool is_light_source_obstructed(const Ray& ray, Interval ray_t, const Point3D& light_position) const override {
            // Empty implementation
            return false;
        }

        std::shared_ptr<Material> getMaterial() const override {
            // Empty implementation
            return nullptr;
        }

        Point3D getCenter() const override {
            // Empty implementation
            return Point3D(0,0,0);
        }

        double getRadius() const override {
            // Empty implementation
            return 0;
        }

        double getHeight() const override {
            // Empty implementation
            return 0;
        }

        virtual void print() const override {
            std::clog << "Scene: " << std::endl;
            for (const auto& shape : shapes) {
                shape->print();
            }
        }

    /**
     * Calculates the lighting for a given hit point based on the provided ray and hit record.
     *
     * @param ray The ray used for calculating lighting.
     * @param record The hit record containing information about the intersection.
     * @return The calculated lighting color for the hit point.
     */
    Color calculateLightingForHitPoint(const Ray& ray, const Hit_record& record) const {
        Color total_light(0, 0, 0);

        for (const auto& light : lights) {
            Point3D light_position = light->getPosition();
            // Calculate light direction and distance to light
            Vector3D light_direction = light_position - record.p;
            double distance_to_light = getLength(light_direction);
            light_direction = normalize(light_direction);
            Color light_color = light->getIntensity();
            int num_shadowrays = 10;
            Color temp_total_light (0,0,0);

            Vector3D view_direction = -ray.getDirection();

            // Shoot shadow rays
            for (int i = 0; i < num_shadowrays; i++) {
                // Get shadow ray
                Ray shadow_ray = getShadowRay(record.p, light_position);
                bool is_shadowed = false;
                
                // Check if shadow ray intersects any of the shapes in the scene
                for (const auto& other_shape : shapes) {
                    std::shared_ptr<Material> mat_ptr = other_shape->getMaterial();
                    // First check if the shape is refractive. If it is, the shadow ray is not obstructed
                    if (mat_ptr->is_refractive()) {
                        // If the shape is refractive, the shadow ray is not obstructed
                        continue;
                    }
                    // If shape is not refractive, do some preliminary checks
                    if (other_shape->is_sphere()) {
                        // Check if the sphere is within the interval of the shadow ray. Within the interval of 0 to distance_to_light + radius of sphere
                        Point3D center = other_shape->getCenter();
                        double radius = other_shape->getRadius();
                        Interval interval(0.001, distance_to_light + radius);
                        double distance_of_center_to_light = getLength(center - light_position);
                        // if the centre of the sphere is not within the interval, then the sphere is not obstructing the light. Skip this sphere
                        if (!interval.contains(distance_of_center_to_light)) {
                            continue;
                        } else {
                            Vector3D shape_to_light = light_position - center;
                            double dot_product = dotProduct(light_direction, shape_to_light);
                            if (dot_product < 0.5) {        // Adjust this threshold if needed
                                // Unlikely to cast a shadow
                                continue;
                            }
                        }
                    } else if (other_shape->is_cylinder()) {
                        // Check if the cylinder is within the interval of the shadow ray. Within the interval of 0 to distance_to_light + radius of cylinder
                        Point3D center = other_shape->getCenter();
                        double radius = other_shape->getRadius();
                        double height = other_shape->getHeight();
                        Interval interval(0.001, distance_to_light + std::max(radius, height));
                        double distance_of_center_to_light = getLength(center - light_position);
                        // if the centre of the cylinder is not within the interval, then the cylinder is not obstructing the light. Skip this cylinder
                        if (!interval.contains(distance_of_center_to_light)) {
                            continue;
                        } else {
                            Vector3D shape_to_light = light_position - center;
                            double dot_product = dotProduct(light_direction, shape_to_light);
                            if (dot_product < 0.5) {        // Adjust this threshold if needed
                                // Unlikely to cast a shadow
                                continue;
                            }
                        }
                    }
                    // If the shape is not refractive and is within the interval of the shadow ray, check if the shadow ray intersects the shape
                    if (other_shape->is_light_source_obstructed(shadow_ray, Interval(0.001, distance_to_light), light->getPosition())) { 
                        is_shadowed = true;
                        break;
                    }
                    
                }

                // If not shadowed, add light contribution
                if (!is_shadowed) {
                    // store in a temporary variable
                    temp_total_light += record.mat_ptr->shade(record, light_direction, view_direction, light_color, distance_to_light);
                } else {
                    // store in a temporary variable
                    temp_total_light += record.mat_ptr->shade(record, light_direction, view_direction, light_color, -1);
                }
                // After all shadow rays have been shot, add the average light contribution to the total light
                total_light += temp_total_light / num_shadowrays;
            }
        }
        // After all lights have been processed, return the total light
        return total_light;
    }

    Ray getShadowRay(const Point3D& hit_point, const Point3D& light_position) const {
        auto sample = light_position + random_in_unit_sphere() * 0.1;

        auto ray_direction = sample - hit_point;
        return Ray(hit_point, ray_direction);
    }
};

#endif // SCENE_H