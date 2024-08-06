#ifndef MATERIAL_H
#define MATERIAL_H

#include "math_utils.h"
#include "color.h"
#include "shape.h"

#include <iostream>

class Hit_record;

class Material {
    public:
        virtual ~Material() = default;

        virtual Color shade(const Hit_record& record, const Vector3D& light_direction, const Vector3D& view_direction, const Color& light_color, double disance_to_light) const = 0;
        virtual bool scatter(const Ray& r_in, const Vector3D& normal, const Vector3D& p, const bool frontFace, Color& attenuation, Ray& scattered, Color& light_contribution) const = 0;
        virtual bool is_refractive() const {return false;}
};

// Matte material
class Lambertian : public Material {

    public:
        Lambertian(const Color& _a) : albedo(_a) {}

        // Lambertian model
        virtual bool scatter(const Ray& r_in, const Vector3D& normal, const Vector3D& p, const bool frontFace, Color& attenuation, Ray& scattered, Color& light_contribution) const override {
            Vector3D scatter_direction = normal + random_in_hemisphere(normal);

            // Keep generating a new scatter direction until it points away from the surface and its length is not zero
            while (dotProduct(scatter_direction, normal) < 0 || getLength(scatter_direction) < 1e-8) {
                scatter_direction = normal + random_in_hemisphere(normal);
            }
            attenuation = light_contribution;
            scattered = Ray(p, scatter_direction);
            return true;
        }

        // Lambertian shading, used for calculating total_light contribution
        virtual Color shade(const Hit_record& record, const Vector3D& light_direction, const Vector3D& view_direction, const Color& light_color, double distance_to_light) const override {
            Color ambient = albedo * 0.05;  // Ambient term, scaled down version of albedo
            if (distance_to_light < 0) {
                // Point is in shadow
                return ambient;
            }
            double cos_theta = std::max(0.0, dotProduct(record.normal, light_direction));
            double attenuation = 1.0 / (distance_to_light * distance_to_light);

            return ambient + light_color * albedo * cos_theta * attenuation;
        }

        // Is refractive?
        virtual bool is_refractive() const override {return false;}

    private:
        Color albedo;
};

        // Blinn-Phong material
        class Blinn_Phong : public Material {
            public:
                Blinn_Phong(const Color& _d, const Color& _s, double _kd, double _ks, int _specular_exponent, bool _is_reflective = false, double _reflectivity = 0.0, bool _is_refractive = false, double _refractiveIndex = 0.0) : diffuseColor(_d), specularColor(_s), kd(_kd), ks(_ks), specular_exponent(_specular_exponent), isReflective(_is_reflective), reflectivity(_reflectivity), isRefractive(_is_refractive), refractiveIndex(_refractiveIndex) {}

        // Blinn-Phong model
        virtual bool scatter(const Ray& r_in, const Vector3D& normal, const Vector3D& p, const bool front_face, Color& attenuation, Ray& scattered, Color& light_contribution) const override {
            Vector3D scatter_direction;
            if (isReflective) {
                // Reflective: only give a reflected ray
                scatter_direction = reflect(normalize(r_in.getDirection()), normal);
                attenuation = light_contribution * reflectivity;
            } else if (isRefractive) {
                attenuation = Color(1.0, 1.0, 1.0);
                double refraction_ratio = front_face ? (1.0 / refractiveIndex) : refractiveIndex;

                Vector3D unit_direction = normalize(r_in.getDirection());
                double cos_theta = fmin(dotProduct(-unit_direction, normal), 1.0);
                double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

                bool cannot_refract = refraction_ratio * sin_theta > 1.0;
                Vector3D direction;

                if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                    direction = reflect(unit_direction, normal);
                else
                    direction = refract(unit_direction, normal, refraction_ratio);

                scattered = Ray(p, direction);
                return true;
            } else {
                // Not reflective: consider specular and diffuse reflection
                if (random_double() < ks) {
                    // Specular reflection
                    scatter_direction = reflect(normalize(r_in.getDirection()), normal);
                } else {
                    // Diffuse reflection
                    scatter_direction = normal + random_in_hemisphere(normal);
                }
                attenuation = light_contribution * kd;
            }

            // Keep generating a new scatter direction until it points away from the surface and its length is not zero
            while (dotProduct(scatter_direction, normal) < 0 || getLength(scatter_direction) < 1e-8) {
                scatter_direction = normal + random_in_hemisphere(normal);
            }

            scattered = Ray(p, scatter_direction);
            return true;
        }

        // Blinn-Phong shading
        virtual Color shade(const Hit_record& record, const Vector3D& light_direction, const Vector3D& view_direction, const Color& light_color, double distance_to_light) const override {

            Color ambient = diffuseColor * 0.05 * kd;  // Ambient term, scaled down version of diffuseColor
            if (distance_to_light < 0) {
                // Point is in shadow
                return ambient;
            }
            Vector3D halfway_vector = normalize(light_direction + view_direction);

            double diffuse = std::max(dotProduct(light_direction, record.normal), 0.0);
            double specular = pow(std::max(dotProduct(halfway_vector, record.normal), 0.0), specular_exponent);

            double attenuation = 1.0 / (distance_to_light * distance_to_light);

            return ambient + light_color * (diffuseColor * kd * diffuse + specularColor * ks * specular) * attenuation;
        }

        // Is refractive?
        virtual bool is_refractive() const override {return isRefractive;}

    private:
        Color diffuseColor;     // diffuse color also used for ambient
        Color specularColor;    // specular color
        double kd;  // diffuse coefficient
        double ks;  // specular coefficient
        int specular_exponent;   // specular exponent
        bool isReflective;      // is the material reflective?
        double reflectivity;    // reflectivity
        bool isRefractive;      // is the material refractive?
        double refractiveIndex;    // refractive index

        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif