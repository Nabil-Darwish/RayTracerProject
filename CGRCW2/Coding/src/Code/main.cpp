#include "math_utils.h"

#include "camera.h"
#include "color.h"
#include "scene_reader.h"
#include "material.h"

#include <iostream>
//#include <crtdbg.h>

int main(int argc, char* argv[]) {
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    std:: string input_file;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        input_file = "default.json"; // Replace with your default file name
    } else {
        input_file = argv[1];
    }

    // Scene
    SceneReader scene_reader(input_file);

    Scene scene = scene_reader.buildScene();

    // TODO: Add your code here to build the scene from the input file
    // The following code is just for testing the materials

    /*auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<Blinn_Phong>(Color(0.5, 0.5, 0.5), Color(1, 1, 1), 1.0, 0.2, 500, false, 1.0, true, 1.5);
    auto material_right  = std::make_shared<Blinn_Phong>(Color(0, 1, 0), Color(1, 1, 1), 0.6, 0.05, 20);
    auto material_reflective = std::make_shared<Blinn_Phong>(Color(0.5, 0.5, 0.5), Color(1, 1, 1), 1.0, 0.2, 500, true, 1.0);
    auto material_triangle = std::make_shared<Lambertian>(Color(0.2, 0.5, 0.8));*/

    //scene.add(std::make_shared<Sphere>(Point3D( 0.0, -100.5, -1.0), 100.0, material_ground));
    //scene.add(std::make_shared<Sphere>(Point3D( 0.0,    0.0, -1.0),   0.5, material_center));
    //scene.add(std::make_shared<Sphere>(Point3D(-1.0,    0.0, -1.0),   0.5, material_left));
    //scene.add(std::make_shared<Sphere>(Point3D(-1.0,    0.0, -1.0), -0.45, material_left));
    //scene.add(std::make_shared<Sphere>(Point3D( 1.0,    0.0, -1.0),   0.5, material_right));
    //scene.add(std::make_shared<Cylinder>(Point3D(1.0, -0.5, -2.0), Vector3D(0.0, 1.0, 0.0), 0.5, 1, material_left));
    //auto material_left  = std::make_shared<Lambertian>(Color(0,0,1));
    //auto material_right = std::make_shared<Lambertian>(Color(1,0,0));
    /*Point3D point1(0.0, 0.0, -1.0);
    Point3D point2(1.0, 0.0, -1.0);
    Point3D point3(0.5, 1.0, -1.0);
    scene.add(std::make_shared<Triangle>(point1, point2, point3, material_reflective));*/

    //scene.add(std::make_shared<Sphere>(Point3D(-R, 0, -1), R, material_left));
    //scene.add(std::make_shared<Sphere>(Point3D( R, 0, -1), R, material_right));

    //scene.add(std::make_shared<PointLight>(Point3D(0, 1, 0), Color(10, 10, 10)));

    auto vector_background_color = scene_reader.getSceneBackgroundColor();

    Color background = Color(vector_background_color[0], vector_background_color[1], vector_background_color[2]);

    // Rendermode
    std::string render_mode = scene_reader.getRenderMode();

    // Camera

    Camera camera;

    camera.modifyCamera(scene_reader);

    /*camera.vfov = 90;
    camera.lookfrom = Point3D(0, 0, 0);
    camera.lookat = Point3D(1, 0, -1);
    camera.vup = Vector3D(0, 1, 0);
    camera.exposure = 0.1;*/

    // Render

    camera.render(scene, background, render_mode);
}