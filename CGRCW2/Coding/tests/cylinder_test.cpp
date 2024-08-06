#include "cylinder.h"

#include <cassert>
#include <iostream>

void testCylinderCaps() {
    Cylinder cylinder(Point3D(0, 0, 0), Vector3D(0, 0, 1), 1, 2);
    Ray rayTopCap(Point3D(0, 0, 2), Vector3D(0, 0, -1));
    Ray rayBottomCap(Point3D(0, 0, -2), Vector3D(0, 0, 1));
    Hit_record record;
    Vector3D normalized_cylinder_axis = normalize(cylinder.getAxis());

    assert(cylinder.hit(rayTopCap, 0, 100, record));
    Vector3D p1_proj = dotProduct(record.p - cylinder.getCenter(), normalized_cylinder_axis) * normalized_cylinder_axis;
    assert(abs(getLength(p1_proj) - cylinder.getHeight()) < 1e-6);

    assert(cylinder.hit(rayBottomCap, 0, 100, record));
    Vector3D p2_proj = dotProduct(record.p - cylinder.getCenter(), normalized_cylinder_axis) * normalized_cylinder_axis;
    assert(abs(getLength(p2_proj)) < 1e-6);
}

int main() {
    std::cout << "Running tests...\n";
    testCylinderCaps();
    std::cout << "Cylinder tests passed!\n";
    return 0;
}