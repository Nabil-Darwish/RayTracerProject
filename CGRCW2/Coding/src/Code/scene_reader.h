#ifndef SCENE_READER_H
#define SCENE_READER_H

#include "scene.h"
#include "nlohmann/json.hpp"
#include "material.h"

#include <fstream>
#include <iostream>

class SceneReader {
public:
    // Constructor that takes a filename
    SceneReader(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
        file >> json;
    }

    // Constructor that takes a JSON object
    SceneReader(const nlohmann::json& jsonInput) {
        json = jsonInput;
    }

    int getNbounces() {
        try {
            return json.at("nbounces").get<int>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'nbounces' not found in JSON file. Using default: 10" << std::endl;
            return 10;
        }
    }

    std::string getRenderMode() {
        try {
            if (json.at("rendermode").is_string()) {
                return json.at("rendermode");
            } else {
                std::cerr << "Error: 'rendermode' must be a string, but is " << json.at("rendermode").type_name() << ". Using default render mode: binary" << std::endl;
                return "binary";
            }
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'rendermode' not found in JSON file. Using default render mode: binary" << std::endl;
            return "binary";
        }
    }

    std::string getCameraType() {
        try {
            return json.at("camera").at("type");
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'camera' or 'type' not found in JSON file. Using default camera type: pinhole" << std::endl;
            return "pinhole";
        }
    }

    int getCameraWidth() {
        if (json.is_null()) {
            return 100;
        }
        nlohmann::json camera = json.value("camera", nlohmann::json());
        if (camera.is_null() || !camera["width"].is_number()) {
            std::cerr << "Error: 'width' is null or not a number in JSON file. Using default camera width: 100" << std::endl;
            return 100;
        }
        return camera["width"].get<int>();
    }

    int getCameraHeight() {
        if (json.is_null()) {
            return 100;
        }
        nlohmann::json camera = json.value("camera", nlohmann::json());
        if (camera.is_null() || !camera["height"].is_number()) {
            std::cerr << "Error: 'height' is null or not a number in JSON file. Using default camera height: 100" << std::endl;
            return 100;
        }
        return camera["height"].get<int>();
    }

    std::vector<double> getCameraPosition() {
        try {
            return json.at("camera").at("position").get<std::vector<double>>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'camera' or 'position' not found in JSON file. Using default camera position: [0,0,0]" << std::endl;
            return {0.0,0.0,0.0};
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'position' is not an array of numbers. Using default camera position: [0,0,0]" << std::endl;
            return {0.0,0.0,0.0};
        }
    }

    std::vector<double> getCameraLookAt() {
        try {
            return json.at("camera").at("lookAt").get<std::vector<double>>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'camera' or 'lookAt' not found in JSON file. Using default camera lookat: [0,0,-1]" << std::endl;
            return {0.0,0.0,-1.0};
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'lookat' is not an array of numbers. Using default camera lookat: [0,0,-1]" << std::endl;
            return {0.0,0.0,-1.0};
        }
    }

    std::vector<double> getCameraUp() {
        try {
            return json.at("camera").at("upVector").get<std::vector<double>>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'camera' or 'upVector' not found in JSON file. Using default camera up: [0,1,0]" << std::endl;
            return {0.0,1.0,0.0};
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'up' is not an array of numbers. Using default camera up: [0,1,0]" << std::endl;
            return {0.0,1.0,0.0};
        }
    }

    double getCameraFov() {
        try {
            return json.at("camera").at("fov").get<double>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'camera' or 'fov' not found in JSON file. Using default camera fov: 90" << std::endl;
            return 90.0;
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'fov' is not a number. Using default camera fov: 90" << std::endl;
            return 90.0;
        }
    }

    double getCameraExposure() {
        try {
            return json.at("camera").at("exposure").get<double>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'camera' or 'exposure' not found in JSON file. Using default camera exposure: 1" << std::endl;
            return 1.0;
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'exposure' is not a number. Using default camera exposure: 1" << std::endl;
            return 1.0;
        }
    
    }

    std::vector<double> getSceneBackgroundColor() {
        try {
            return json.at("scene").at("backgroundcolor").get<std::vector<double>>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'scene' or 'backgroundcolor' not found in JSON file. Using default scene backgroundcolor: [0,0,0]" << std::endl;
            return {0.0,0.0,0.0};
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'backgroundcolor' is not an array of numbers. Using default scene backgroundcolor: [0,0,0]" << std::endl;
            return {0.0,0.0,0.0};
        }
    }

    std::vector<nlohmann::json> getLightSources() {
        if (json.is_null()) {
            std::cerr << "Error: JSON file is null. Using default scene lightsources: []" << std::endl;
            return {};
        }

        try {
            return json.at("scene").at("lightsources").get<std::vector<nlohmann::json>>();
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'scene' or 'lightsources' not found in JSON file. Using default scene lightsources: []" << std::endl;
            return {};
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'lightsources' is not an array. Using default scene lightsources: []" << std::endl;
            return {};
        }
    }

    std::vector<nlohmann::json> getShapes() {
        if (json.is_null()) {
            std::cerr << "Error: JSON file is null. Using default scene shapes: []" << std::endl;
            return {};
        }

        try {
            nlohmann::json scene = json.at("scene");
            if (scene.is_null()) {
                std::cerr << "Error: 'scene' is null in JSON file. Using default scene shapes: []" << std::endl;
                return {};
            }

            std::vector<nlohmann::json> shapes = scene.at("shapes").get<std::vector<nlohmann::json>>();
            if (shapes.empty()) {
                std::cerr << "Error: 'shapes' is null or empty in JSON file. Using default scene shapes: []" << std::endl;
                return {};
            }

            return shapes;
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'shapes' not found in JSON file. Using default scene shapes: []" << std::endl;
            return {};
        } catch (nlohmann::json::type_error& e) {
            std::cerr << "Error: 'shapes' is not an array. Using default scene shapes: []" << std::endl;
            return {};
        }
    }

    nlohmann::json getMaterialData(const nlohmann::json& shape) {
        try {
            return shape.at("material");
        } catch (nlohmann::json::out_of_range& e) {
            std::cerr << "Error: 'material' not found in shape data. Using default material." << std::endl;
            return nlohmann::json();
        }
    }

    Scene buildScene() {
        Scene scene;
        std::vector<nlohmann::json> lights = getLightSources();
        std::vector<nlohmann::json> shapes = getShapes();

        for (const nlohmann::json& light : lights) {
            try {
                std::string type = light.at("type").get<std::string>();
                if (type == "pointlight") {
                    std::vector<double> position = light.at("position").get<std::vector<double>>();
                    std::vector<double> intensity = light.at("intensity").get<std::vector<double>>();
                    scene.add(std::make_shared<PointLight>(Point3D(position[0], position[1], position[2]), Color(intensity[0], intensity[1], intensity[2])));
                    //std::clog << "Added point light at position (" << position[0] << ", " << position[1] << ", " << position[2] << ")\n";
                } else {
                    std::cerr << "Error: light type '" << type << "' not recognized. Skipping light." << std::endl;
                }
            } catch (nlohmann::json::out_of_range& e) {
                std::cerr << "Error: required key not found in light. Skipping this light." << std::endl;
            } catch (nlohmann::json::type_error& e) {
                std::cerr << "Error: incorrect type for a key in light. Skipping this light." << std::endl;
            }
        }

        for (const nlohmann::json& shape : shapes) {
            try {
    
                nlohmann::json material_data = getMaterialData(shape);
                std::shared_ptr<Material> material;
                if (material_data.empty()) {        // Default material
                    std::make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
                    material = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
                } else if (!material_data.is_object()) {
                    std::cerr << "Error: material data is not an object. Skipping shape." << std::endl;
                    continue;
                } else {
                    double ks = shape.at("material").at("ks").get<double>();
                    double kd = shape.at("material").at("kd").get<double>();
                    int specularexponent = shape.at("material").at("specularexponent").get<int>();
                    std::vector<double> diffusecolor = shape.at("material").at("diffusecolor").get<std::vector<double>>();
                    std::vector<double> specularcolor = shape.at("material").at("specularcolor").get<std::vector<double>>();
                    bool isreflective = shape.at("material").at("isreflective").get<bool>();
                    double reflectivity = shape.at("material").at("reflectivity").get<double>();
                    bool isrefractive = shape.at("material").at("isrefractive").get<bool>();
                    double refractiveindex = shape.at("material").at("refractiveindex").get<double>();
                    material = std::make_shared<Blinn_Phong>(Color(diffusecolor[0], diffusecolor[1], diffusecolor[2]), Color(specularcolor[0], specularcolor[1], specularcolor[2]), kd, ks, specularexponent, isreflective, reflectivity, isrefractive, refractiveindex);
                }
                //std::clog << "Material created...\n";
                std::string type = shape.at("type").get<std::string>();
                //std::clog << "Shape type: " << type << "\n";
                if (type == "sphere") {
                    std::vector<double> center = shape.at("center").get<std::vector<double>>();
                   // std:: clog << "Center: (" << center[0] << ", " << center[1] << ", " << center[2] << ")\n";
                    double radius = shape.at("radius").get<double>();
                   // std::clog << "Radius: " << radius << "\n";
                    scene.add(std::make_shared<Sphere>(Point3D(center[0], center[1], center[2]), radius, material));
                } else if (type == "cylinder") {
                    std::vector<double> center = shape.at("center").get<std::vector<double>>();
                   // std::clog << "Center: (" << center[0] << ", " << center[1] << ", " << center[2] << ")\n";
                    std::vector<double> axis = shape.at("axis").get<std::vector<double>>();
                    //std::clog << "Axis: (" << axis[0] << ", " << axis[1] << ", " << axis[2] << ")\n";
                    double radius = shape.at("radius").get<double>();
                   // std::clog << "Radius: " << radius << "\n";
                    double height = shape.at("height").get<double>();
                    //std::clog << "Height: " << height << "\n";
                    scene.add(std::make_shared<Cylinder>(Point3D(center[0], center[1], center[2]), Vector3D(axis[0], axis[1], axis[2]), radius, height, material));
                } else if (type == "triangle") {
                    std::vector<double> vertex1 = shape.at("v0").get<std::vector<double>>();
                   // std::clog << "Vertex 1: (" << vertex1[0] << ", " << vertex1[1] << ", " << vertex1[2] << ")\n";
                    std::vector<double> vertex2 = shape.at("v1").get<std::vector<double>>();
                    //std::clog << "Vertex 2: (" << vertex2[0] << ", " << vertex2[1] << ", " << vertex2[2] << ")\n";
                    std::vector<double> vertex3 = shape.at("v2").get<std::vector<double>>();
                    //std::clog << "Vertex 3: (" << vertex3[0] << ", " << vertex3[1] << ", " << vertex3[2] << ")\n";
                    scene.add(std::make_shared<Triangle>(Point3D(vertex1[0], vertex1[1], vertex1[2]), Point3D(vertex2[0], vertex2[1], vertex2[2]), Point3D(vertex3[0], vertex3[1], vertex3[2]), material));
                } else {
                    std::cerr << "Error: shape type '" << type << "' not recognized. Skipping shape." << std::endl;
                }
            } catch (nlohmann::json::out_of_range& e) {
                std::cerr << "Error: required key not found in shape. Skipping this shape." << std::endl;
            } catch (nlohmann::json::type_error& e) {
                std::cerr << "Error: incorrect type for a key in shape. Skipping this shape." << std::endl;
            }
        }

        //std::clog << "Scene built.\n";
        return scene;
    }

    void modifyScene(Scene& scene) {
        std::vector<nlohmann::json> shapes = getShapes();
        std::vector<nlohmann::json> lights = getLightSources();
        for (const nlohmann::json& light : lights) {
            try {
                std::string type = light.at("type").get<std::string>();
                if (type == "pointlight") {
                    std::vector<double> position = light.at("position").get<std::vector<double>>();
                    std::vector<double> intensity = light.at("intensity").get<std::vector<double>>();
                    scene.add(std::make_shared<PointLight>(Point3D(position[0], position[1], position[2]), Color(intensity[0], intensity[1], intensity[2])));
                } else {
                    std::cerr << "Error: light type '" << type << "' not recognized. Skipping light." << std::endl;
                }
            } catch (nlohmann::json::out_of_range& e) {
                std::cerr << "Error: required key not found in light. Skipping this light." << std::endl;
            } catch (nlohmann::json::type_error& e) {
                std::cerr << "Error: incorrect type for a key in light. Skipping this light." << std::endl;
            }
        }
        for (const nlohmann::json& shape : shapes) {
            try {
                nlohmann::json material_data = getMaterialData(shape);
                std::shared_ptr<Material> material;
                if (material_data.empty()) {        // Default material
                    std::make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
                    material = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
                } else if (!material_data.is_object()) {
                    std::cerr << "Error: material data is not an object. Skipping shape." << std::endl;
                    continue;
                } else {
                    double ks = shape.at("material").at("ks").get<double>();
                    double kd = shape.at("material").at("kd").get<double>();
                    int specularexponent = shape.at("material").at("specularexponent").get<int>();
                    std::vector<double> diffusecolor = shape.at("material").at("diffusecolor").get<std::vector<double>>();
                    std::vector<double> specularcolor = shape.at("material").at("specularcolor").get<std::vector<double>>();
                    bool isreflective = shape.at("material").at("isreflective").get<bool>();
                    double reflectivity = shape.at("material").at("reflectivity").get<double>();
                    bool isrefractive = shape.at("material").at("isrefractive").get<bool>();
                    double refractiveindex = shape.at("material").at("refractiveindex").get<double>();
                    material = std::make_shared<Blinn_Phong>(Color(diffusecolor[0], diffusecolor[1], diffusecolor[2]), Color(specularcolor[0], specularcolor[1], specularcolor[2]), kd, ks, specularexponent, isreflective, reflectivity, isrefractive, refractiveindex);
                }
                std::string type = shape.at("type").get<std::string>();
                if (type == "sphere") {
                    std::vector<double> center = shape.at("center").get<std::vector<double>>();
                    double radius = shape.at("radius").get<double>();
                    scene.add(std::make_shared<Sphere>(Point3D(center[0], center[1], center[2]), radius, material));
                } else if (type == "cylinder") {
                    std::vector<double> center = shape.at("center").get<std::vector<double>>();
                    std::vector<double> axis = shape.at("axis").get<std::vector<double>>();
                    double radius = shape.at("radius").get<double>();
                    double height = shape.at("height").get<double>();
                    scene.add(std::make_shared<Cylinder>(Point3D(center[0], center[1], center[2]), Vector3D(axis[0], axis[1], axis[2]), radius, height, material));
                } else if (type == "triangle") {
                    std::vector<double> vertex1 = shape.at("v0").get<std::vector<double>>();
                    std::vector<double> vertex2 = shape.at("v1").get<std::vector<double>>();
                    std::vector<double> vertex3 = shape.at("v2").get<std::vector<double>>();
                    scene.add(std::make_shared<Triangle>(Point3D(vertex1[0], vertex1[1], vertex1[2]), Point3D(vertex2[0], vertex2[1], vertex2[2]), Point3D(vertex3[0], vertex3[1], vertex3[2]), material));
                } else {
                    std::cerr << "Error: shape type '" << type << "' not recognized. Skipping shape." << std::endl;
                }
            } catch (nlohmann::json::out_of_range& e) {
                std::cerr << "Error: key not found in shape. Skipping shape." << std::endl;
            } catch (nlohmann::json::type_error& e) {
                std::cerr << "Error: incorrect type for a key in shape. Skipping this shape." << std::endl;
            }
        }
    }


    // TODO: Implement methods to get camera and scene details

private:
    nlohmann::json json;
};

#endif // SCENE_READER_H
/*
*/