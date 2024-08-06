#include <cassert>
#include "vector.h"
#include <iostream>

void test_vector_addition() {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    Vector3D result = v1 + v2;

    assert(result.x == 5.0);
    assert(result.y == 7.0);
    assert(result.z == 9.0);
}

void test_vector_subtraction() {
    Vector3D v1(4.0, 5.0, 6.0);
    Vector3D v2(1.0, 2.0, 3.0);
    Vector3D result = v1 - v2;

    assert(result.x == 3.0);
    assert(result.y == 3.0);
    assert(result.z == 3.0);
}

void test_vector_cross_product() {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    Vector3D result = crossProduct(v1, v2);

    assert(result.x == -3.0);
    assert(result.y == 6.0);
    assert(result.z == -3.0);
}

void test_vector_dot_product() {
    Vector3D v1(1.0, 2.0, 3.0);
    Vector3D v2(4.0, 5.0, 6.0);
    double result = dotProduct(v1, v2);

    assert(result == 32.0);
}

void test_vector_scalar_multiplication() {
    Vector3D v(1.0, 2.0, 3.0);
    Vector3D result = v * 2.0;

    assert(result.x == 2.0);
    assert(result.y == 4.0);
    assert(result.z == 6.0);
}

void test_vector_scalar_multiplication_left_scalar() {
    Vector3D v(1.0, 2.0, 3.0);
    Vector3D result = 2.0 * v;

    assert(result.x == 2.0);
    assert(result.y == 4.0);
    assert(result.z == 6.0);

} 

void test_vector_scalar_division() {
    Vector3D v(2.0, 4.0, 6.0);
    Vector3D result = v / 2.0;

    assert(result.x == 1.0);
    assert(result.y == 2.0);
    assert(result.z == 3.0);
}

void test_vector_length_squared() {
    Vector3D v(1.0, 2.0, 3.0);
    double result = getLengthSquared(v);

    assert(result == 14.0);
}

void test_vector_length() {
    Vector3D v(1.0, 2.0, 2.0);
    double result = getLength(v);

    assert(result == 3.0);
}

void test_vector_normalization() {
    Vector3D v(1.0, 2.0, 2.0);
    Vector3D result = normalize(v);

    assert(result.x == 1.0 / 3.0);
    assert(result.y == 2.0 / 3.0);
    assert(result.z == 2.0 / 3.0);
}

int main() {
    std::cout << "Running vector tests..." << std::endl;
    test_vector_addition();
    std::cout << "Vector addition test passed!" << std::endl;
    test_vector_subtraction();
    std::cout << "Vector subtraction test passed!" << std::endl;
    test_vector_cross_product();
    std::cout << "Vector cross product test passed!" << std::endl;
    test_vector_dot_product();
    std::cout << "Vector dot product test passed!" << std::endl;
    test_vector_scalar_multiplication();
    std::cout << "Vector scalar multiplication test passed!" << std::endl;
    test_vector_scalar_multiplication_left_scalar();
    std::cout << "Vector scalar multiplication left scalar test passed!" << std::endl;
    test_vector_scalar_division();
    std::cout << "Vector scalar division test passed!" << std::endl;
    test_vector_length_squared();
    std::cout << "Vector length squared test passed!" << std::endl;
    test_vector_length();
    std::cout << "Vector length test passed!" << std::endl;
    test_vector_normalization();
    std::cout << "Vector normalization test passed!" << std::endl;

    return 0;
}