#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"

#include "color.h"
#include "scene_reader.h"
#include "material.h"

#include <iostream>

class Camera {
  public:
    /* Public Camera Parameters Here */

    double      aspect_ratio = 16.0 / 9.0;
    int         image_width = 400;
    int         samples_per_pixel = 30;
    int         max_depth = 8;
    double      exposure = 1.0;

    double      vfov        = 90;  // Vertical view angle (field of view)
    Point3D     lookfrom    = Point3D(0,0,-1);  // Point camera is looking from
    Point3D     lookat      = Point3D(0,0,0);   // Point camera is looking at
    Vector3D    vup         = Vector3D(0,1,0);     // Camera-relative "up" direction


    void modifyCamera(SceneReader& scene_reader) {
        image_width = scene_reader.getCameraWidth();
        aspect_ratio = static_cast<double>(image_width) / static_cast<double>(scene_reader.getCameraHeight());
        max_depth = scene_reader.getNbounces();
        exposure = scene_reader.getCameraExposure();

        vfov = scene_reader.getCameraFov();
        auto cameraPos = scene_reader.getCameraPosition();
        lookfrom = Point3D(cameraPos[0], cameraPos[1], cameraPos[2]);
        auto cameraDir = scene_reader.getCameraLookAt();
        lookat = Point3D(cameraDir[0], cameraDir[1], cameraDir[2]);
        auto cameraUp = scene_reader.getCameraUp();
        vup = Vector3D(cameraUp[0], cameraUp[1], cameraUp[2]);

        //Later, we will add more parameters here*/
        // Functions to do: getMaxDepth, getSamplesPerPixel, getAspectRatio, getImageWidth
    }

    void render(const Scene& scene, const Color&background, const std::string& render_mode) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, scene, background, render_mode);
                }
                pixel_color = exposure * pixel_color;
                writeColor(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\rDone.                 \n";
    }

  private:
    /* Private Camera Variables Here */

    int         image_height;   // Rendered image height
    Point3D     center;         // Camera center
    Point3D     pixel00_loc;    // Location of pixel 0, 0
    Vector3D    pixel_delta_u;  // Offset to pixel to the right
    Vector3D    pixel_delta_v;  // Offset to pixel below
    Vector3D    u, v, w;        // Camera coordinate system
    /*double      viewport_height;//
    double      viewport_width;*/

    void initialize() {
      image_height = static_cast<int>(image_width / aspect_ratio);
      image_height = (image_height < 1) ? 1 : image_height;

      center = lookfrom;

      // Determine viewport size
      auto focal_length = getLength(lookfrom - lookat);
      auto theta = degrees_to_radians(vfov);
      auto h = tan(theta / 2);
      auto viewport_height = 2 * h * focal_length;
      auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

      // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
      w = normalize(lookfrom - lookat);
      u = normalize(crossProduct(vup, w));
      v = crossProduct(w, u);

      // Calculate vectors across horizontal and down vertical
      auto horizontal = viewport_width * u;
      auto vertical = viewport_height * -v;

      // Calculate pixel deltas
      pixel_delta_u = horizontal / (image_width - 1);
      pixel_delta_v = vertical / (image_height - 1);

      // Calculate location of upper left pixel
      auto viewport_upper_left = center - focal_length * w - 0.5 * (horizontal + vertical);
      pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    Ray get_ray(int i, int j) const {
        auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
        auto pixel_sample = pixel_center + pixel_sample_disk();

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;
        return Ray(ray_origin, ray_direction);
    }

    Vector3D pixel_sample_disk() const {
        // Returns random point in the pixel disk
        double theta = 2 * pi * random_double();  // Random angle
        double r = sqrt(random_double());  // Random radius
        auto px = r * cos(theta);
        auto py = r * sin(theta);
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    Vector3D pixel_sample_square() const {
        // Returns random point in the pixel square
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    Color ray_color(const Ray& r, int depth, const Scene& scene, const Color background, const std::string render_mode) const {
        Hit_record rec;

        double diffuseFactor = 0.5;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0) {
            return background;          // Might change to black
        }


        // binary returns red if hit, normal returns normal as color, diffuse returns random diffuse color
        if (scene.hit(r, Interval(0.001, infinity), rec)) {
            if(render_mode == "binary") {
                return Color(1, 0, 0);
            } else if (render_mode == "normal") {
                return 0.5 * (rec.normal + Color(1,1,1));
            } else if (render_mode == "diffuse") {
                Vector3D direction = random_in_hemisphere(rec.normal);
                return diffuseFactor * ray_color(Ray(rec.p, direction), depth-1, scene, background, render_mode);
            } else if (render_mode == "phong") {
                Ray scattered;
                Color attenuation;
                Color light_contribution = scene.calculateLightingForHitPoint(r, rec);
                if (rec.mat_ptr->scatter(r, rec.normal, rec.p, rec.front_face, attenuation, scattered, light_contribution)) {
                    return attenuation * ray_color(scattered, depth-1, scene, background, render_mode);
                }
                return Color(0, 1, 0);
            } else {
                std::cerr << "Error: Invalid render mode '" << render_mode << "'" << std::endl;
                exit(1);
            }
        } else {
            return background;
        }
    }
};

#endif