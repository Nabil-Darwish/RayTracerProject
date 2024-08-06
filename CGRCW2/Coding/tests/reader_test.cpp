#include "scene_reader.h"
#include "cylinder.h"
#include "sphere.h"
#include "triangle.h"
#include <cassert>

void testGetRenderMode() {
    nlohmann::json json = {{"rendermode", "phong"}};
    SceneReader reader(json);
    std::string renderMode = reader.getRenderMode();
    assert(renderMode == "phong");
}

void testGetCameraType() {
    nlohmann::json json = {{"camera", {{"type", "thinlens"}}}};
    SceneReader reader(json);
    std::string cameraType = reader.getCameraType();
    assert(cameraType == "thinlens");
}

void testGetCameraWidth() {
    nlohmann::json json = {{"camera", {{"width", 800}}}};
    SceneReader reader(json);
    int cameraWidth = reader.getCameraWidth();
    assert(cameraWidth == 800);
}

void testGetCameraHeight() {
    nlohmann::json json = {{"camera", {{"height", 600}}}};
    SceneReader reader(json);
    int cameraHeight = reader.getCameraHeight();
    assert(cameraHeight == 600);
}

void testGetCameraPosition() {
    nlohmann::json json = {{"camera", {{"position", {1.0, 2.0, 3.0}}}}};
    SceneReader reader(json);
    std::vector<double> cameraPosition = reader.getCameraPosition();
    assert(cameraPosition[0] == 1.0);
    assert(cameraPosition[1] == 2.0);
    assert(cameraPosition[2] == 3.0);
}

void testGetCameraLookAt() {
    nlohmann::json json = {{"camera", {{"lookat", {1.0, 2.0, 3.0}}}}};
    SceneReader reader(json);
    std::vector<double> cameraLookAt = reader.getCameraLookAt();
    assert(cameraLookAt[0] == 1.0);
    assert(cameraLookAt[1] == 2.0);
    assert(cameraLookAt[2] == 3.0);
}

void testGetCameraUp() {
    nlohmann::json json = {{"camera", {{"up", {1.0, 2.0, 3.0}}}}};
    SceneReader reader(json);
    std::vector<double> cameraUp = reader.getCameraUp();
    assert(cameraUp[0] == 1.0);
    assert(cameraUp[1] == 2.0);
    assert(cameraUp[2] == 3.0);
}

void testGetCameraFov() {
    nlohmann::json json = {{"camera", {{"fov", 180.0}}}};
    SceneReader reader(json);
    double cameraFov = reader.getCameraFov();
    assert(cameraFov == 180.0);
}

void testGetCameraExposure() {
    nlohmann::json json = {{"camera", {{"exposure", 0.5}}}};
    SceneReader reader(json);
    double cameraExposure = reader.getCameraExposure();
    assert(cameraExposure == 0.5);
}

void testGetSceneBackgroundColor() {
    nlohmann::json json = {{"scene", {{"backgroundcolor", {0.5, 0.75, 0.35}}}}};
    SceneReader reader(json);
    std::vector<double> backgroundColor = reader.getSceneBackgroundColor();
    assert(backgroundColor[0] == 0.5);
    assert(backgroundColor[1] == 0.75);
    assert(backgroundColor[2] == 0.35);
}

void testGetShapes() {
    nlohmann::json json = {
        {"shapes", {
            {{"type", "sphere"}, {"center", {0.0, 0.0, 0.0}}, {"radius", 1.0}},
            {{"type", "cylinder"}, {"center", {1.0, 2.0, 3.0}}, {"axis", {4.0, 5.0, 6.0}}, {"radius", 2.0}, {"height", 3.0}}
        }}
    };
    SceneReader reader(json);
    std::vector<nlohmann::json> shapes = reader.getShapes();
    assert(shapes.size() == 2);
    assert(shapes[0]["type"] == "sphere");
    assert(shapes[0]["center"][0] == 0.0);
    assert(shapes[0]["center"][1] == 0.0);
    assert(shapes[0]["center"][2] == 0.0);
    assert(shapes[0]["radius"] == 1.0);
    assert(shapes[1]["type"] == "cylinder");
    assert(shapes[1]["center"][0] == 1.0);
    assert(shapes[1]["center"][1] == 2.0);
    assert(shapes[1]["center"][2] == 3.0);
    assert(shapes[1]["axis"][0] == 4.0);
    assert(shapes[1]["axis"][1] == 5.0);
    assert(shapes[1]["axis"][2] == 6.0);
    assert(shapes[1]["radius"] == 2.0);
    assert(shapes[1]["height"] == 3.0);
}

void testModifyScene() {
    nlohmann::json json = {
        {"camera", {{"type", "pinhole"}, {"width", 800}, {"height", 600}, {"fov", 90.0}}},
        {"scene", {{"backgroundcolor", {0.0, 0.0, 0.0}}}},
        {"shapes", {
            {{"type", "sphere"}, {"center", {0.0, 0.0, 0.0}}, {"radius", 1.0}},
            {{"type", "cylinder"}, {"center", {1.0, 2.0, 3.0}}, {"axis", {4.0, 5.0, 6.0}}, {"radius", 2.0}, {"height", 3.0}}
        }}
    };
    SceneReader reader(json);
    Scene scene;
    reader.modifyScene(scene);
    assert(scene.shapes.size() == 2);
    Sphere* sphere = dynamic_cast<Sphere*>(scene.shapes[0].get());
    assert(sphere != nullptr);
    assert(sphere->getCenter() == Point3D(0.0, 0.0, 0.0));
    assert(sphere->getRadius() == 1.0);
    Cylinder* cylinder = dynamic_cast<Cylinder*>(scene.shapes[1].get());
    assert(cylinder != nullptr);
    assert(cylinder->getCenter() == Point3D(1.0, 2.0, 3.0));
    assert(cylinder->getAxis() == Vector3D(4.0, 5.0, 6.0));
    assert(cylinder->getRadius() == 2.0);
    assert(cylinder->getHeight() == 3.0);
}

void testBuildScene() {
    nlohmann::json json = {
        {"camera", {{"type", "pinhole"}, {"width", 800}, {"height", 600}, {"fov", 90.0}}},
        {"scene", {{"backgroundcolor", {0.0, 0.0, 0.0}}}},
        {"shapes", {
            {{"type", "sphere"}, {"center", {0.0, 0.0, 0.0}}, {"radius", 1.0}},
            {{"type", "cylinder"}, {"center", {1.0, 2.0, 3.0}}, {"axis", {4.0, 5.0, 6.0}}, {"radius", 2.0}, {"height", 3.0}}
        }}
    };
    SceneReader reader(json);
    Scene scene = reader.buildScene();
    assert(scene.shapes.size() == 2);
    Sphere* sphere = dynamic_cast<Sphere*>(scene.shapes[0].get());
    assert(sphere != nullptr);
    assert(sphere->getCenter() == Point3D(0.0, 0.0, 0.0));
    assert(sphere->getRadius() == 1.0);
    Cylinder* cylinder = dynamic_cast<Cylinder*>(scene.shapes[1].get());
    assert(cylinder != nullptr);
    assert(cylinder->getCenter() == Point3D(1.0, 2.0, 3.0));
    assert(cylinder->getAxis() == Vector3D(4.0, 5.0, 6.0));
    assert(cylinder->getRadius() == 2.0);
    assert(cylinder->getHeight() == 3.0);
}

void testFileNotFound() {
    try {
        SceneReader reader(std::string("path/to/nonexistent/file.json"));
        // If the above line didn't throw an exception, fail the test
        assert(false);
    } catch (std::runtime_error& e) {
        // If a runtime_error was thrown, the test passes
        assert(true);
    }
}

void testGetCameraTypeNoCamera() {
    nlohmann::json json = {{"rendermode", "binary"}};
    SceneReader reader(json);
    std::string cameraType = reader.getCameraType();
    assert(cameraType == "pinhole");
}

void testGetCameraWidthNoCamera() {
    nlohmann::json json = {};
    SceneReader reader(json);
    int width = reader.getCameraWidth();
    assert(width == 100);  // Replace 100 with your default width
}

void testGetCameraWidthNotNumber() {
    nlohmann::json json = {{"camera", {{"width", "not a number"}}}};
    SceneReader reader(json);
    assert(reader.getCameraWidth() == 100); // Default width is 100
}

void testGetCameraHeightNoCamera() {
    nlohmann::json json = {};
    SceneReader reader(json);
    assert(reader.getCameraHeight() == 100); // Default height is 100
}

void testGetCameraHeightNotNumber() {
    nlohmann::json json = {{"camera", {{"height", "not a number"}}}};
    SceneReader reader(json);
    assert(reader.getCameraHeight() == 100); // Default height is 100
}

void testGetCameraPositionNoCamera() {
    nlohmann::json json = {};
    SceneReader reader(json);
    std::vector<double> cameraPosition = reader.getCameraPosition();
    assert(cameraPosition[0] == 0.0);
    assert(cameraPosition[1] == 0.0);
    assert(cameraPosition[2] == 0.0);
}

void testGetCameraPositionNotArray() {
    nlohmann::json json = {{"camera", {{"position", "not an array"}}}};
    SceneReader reader(json);
    std::vector<double> cameraPosition = reader.getCameraPosition();
    assert(cameraPosition[0] == 0.0);
    assert(cameraPosition[1] == 0.0);
    assert(cameraPosition[2] == 0.0);
}

void testGetCameraLookAtNoCamera() {
    nlohmann::json json = {};
    SceneReader reader(json);
    std::vector<double> cameraLookAt = reader.getCameraLookAt();
    assert(cameraLookAt[0] == 0.0);
    assert(cameraLookAt[1] == 0.0);
    assert(cameraLookAt[2] == -1.0); // Default lookAt is {0.0, 0.0, -1.0}
}

void testGetCameraLookAtNotArray() {
    nlohmann::json json = {{"camera", {{"lookat", "not an array"}}}};
    SceneReader reader(json);
    std::vector<double> cameraLookAt = reader.getCameraLookAt();
    assert(cameraLookAt[0] == 0.0);
    assert(cameraLookAt[1] == 0.0);
    assert(cameraLookAt[2] == -1.0); // Default lookAt is {0.0, 0.0, -1.0}
}

void testGetCameraUpNoCamera() {
    nlohmann::json json = {};
    SceneReader reader(json);
    std::vector<double> cameraUp = reader.getCameraUp();
    assert(cameraUp[0] == 0.0);
    assert(cameraUp[1] == 1.0); // Default up is {0.0, 1.0, 0.0}
    assert(cameraUp[2] == 0.0);
}

void testGetCameraUpNotArray() {
    nlohmann::json json = {{"camera", {{"up", "not an array"}}}};
    SceneReader reader(json);
    std::vector<double> cameraUp = reader.getCameraUp();
    assert(cameraUp[0] == 0.0);
    assert(cameraUp[1] == 1.0); // Default up is {0.0, 1.0, 0.0}
    assert(cameraUp[2] == 0.0);
}

void testGetCameraExposureNoCamera() {
    nlohmann::json json = {{"rendermode", "binary"}};
    SceneReader reader(json);
    double cameraExposure = reader.getCameraExposure();
    assert(cameraExposure == 1.0);
}

void testGetCameraExposureNotNumber() {
    nlohmann::json json = {{"camera", {{"exposure", "not a number"}}}};
    SceneReader reader(json);
    double cameraExposure = reader.getCameraExposure();
    assert(cameraExposure == 1.0);
}

void testGetSceneBackgroundColorNoScene() {
    nlohmann::json json = {{"rendermode", "binary"}};
    SceneReader reader(json);
    std::vector<double> backgroundColor = reader.getSceneBackgroundColor();
    assert(backgroundColor[0] == 0);
    assert(backgroundColor[1] == 0);
    assert(backgroundColor[2] == 0);
}

void testGetSceneBackgroundColorNotArray() {
    nlohmann::json json = {{"scene", {{"backgroundcolor", "not an array"}}}};
    SceneReader reader(json);
    std::vector<double> backgroundColor = reader.getSceneBackgroundColor();
    assert(backgroundColor[0] == 0);
    assert(backgroundColor[1] == 0);
    assert(backgroundColor[2] == 0);
}

void testGetShapesNoShapes() {
    nlohmann::json json = {{"rendermode", "binary"}};
    SceneReader reader(json);
    std::vector<nlohmann::json> shapes = reader.getShapes();
    assert(shapes.empty());
}

void testGetShapesNotArray() {
    nlohmann::json json = {{"shapes", "not an array"}};
    SceneReader reader(json);
    std::vector<nlohmann::json> expected = {};
    assert(reader.getShapes() == expected);
}

void testGetCameraFovNoCamera() {
    nlohmann::json json = {};
    SceneReader reader(json);
    assert(reader.getCameraFov() == 90.0);
}

void testGetCameraFovNotNumber() {
    nlohmann::json json = {{"camera", {{"fov", "not a number"}}}};
    SceneReader reader(json);
    assert(reader.getCameraFov() == 90.0);
}

void testFileInput() {
    // Write a JSON object to a file
    nlohmann::json json = {{"rendermode", "phong"}};
    std::ofstream file("test.json");
    file << json;
    file.close();

    // Read the file using SceneReader
    SceneReader reader(std::string("test.json"));
    std::string renderMode = reader.getRenderMode();
    assert(renderMode == "phong");

    // Clean up the test file
    std::remove("test.json");
}

int main() {
    std::cout << "Running tests...\n";
    std::cout << "testGetRenderMode...\n";
    testGetRenderMode();
    std::cout << "testGetRenderMode passed.\n";
    testGetCameraType();
    std::cout << "testGetCameraType passed.\n";
    testGetCameraWidth();
    std::cout << "testGetCameraWidth passed.\n";
    testGetCameraHeight();
    std::cout << "testGetCameraHeight passed.\n";
    testGetCameraPosition();
    std::cout << "testGetCameraPosition passed.\n";
    testGetCameraLookAt();
    std::cout << "testGetCameraLookAt passed.\n";
    testGetCameraUp();
    std::cout << "testGetCameraUp passed.\n";
    testGetCameraFov();
    std::cout << "testGetCameraFov passed.\n";
    testGetCameraExposure();
    std::cout << "testGetCameraExposure passed.\n";
    testGetSceneBackgroundColor();
    std::cout << "testGetSceneBackgroundColor passed.\n";
    testGetShapes();
    std::cout << "testGetShapes passed.\n";
    testModifyScene();
    std::cout << "testBuildScene passed.\n";
    testFileNotFound();
    std::cout << "testFileNotFound passed.\n";
    testGetCameraTypeNoCamera();
    std::cout << "testGetCameraTypeNoCamera passed.\n";
    testGetCameraWidthNoCamera();
    std::cout << "testGetCameraWidthNoCamera passed.\n";
    testGetCameraWidthNotNumber();
    std::cout << "testGetCameraWidthNotNumber passed.\n";
    testGetCameraHeightNoCamera();
    std::cout << "testGetCameraHeightNoCamera passed.\n";
    testGetCameraHeightNotNumber();
    std::cout << "testGetCameraHeightNotNumber passed.\n";
    testGetCameraPositionNoCamera();
    std::cout << "testGetCameraPositionNoCamera passed.\n";
    testGetCameraPositionNotArray();
    std::cout << "testGetCameraPositionNotArray passed.\n";
    testGetCameraLookAtNoCamera();
    std::cout << "testGetCameraLookAtNoCamera passed.\n";
    testGetCameraLookAtNotArray();
    std::cout << "testGetCameraLookAtNotArray passed.\n";
    testGetCameraUpNoCamera();
    std::cout << "testGetCameraUpNoCamera passed.\n";
    testGetCameraUpNotArray();
    std::cout << "testGetCameraUpNotArray passed.\n";
    testGetCameraFovNoCamera();
    std::cout << "testGetCameraFovNoCamera passed.\n";
    testGetCameraFovNotNumber();
    std::cout << "testGetCameraFovNotNumber passed.\n";
    testGetCameraExposureNoCamera();
    std::cout << "testGetCameraExposureNoCamera passed.\n";
    testGetCameraExposureNotNumber();
    std::cout << "testGetCameraExposureNotNumber passed.\n";
    testGetSceneBackgroundColorNoScene();
    std::cout << "testGetSceneBackgroundColorNoScene passed.\n";
    testGetSceneBackgroundColorNotArray();
    std::cout << "testGetSceneBackgroundColorNotArray passed.\n";
    testGetShapesNoShapes();
    std::cout << "testGetShapesNoShapes passed.\n";
    testGetShapesNotArray();
    std::cout << "testGetShapesNotArray passed.\n";
    testFileInput();
    std::cout << "testFileInput passed.\n";
    std::cout << "All tests passed.\n";
    return 0;
}