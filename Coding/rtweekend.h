#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

//Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 355.0/113.0;

//Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi /180.0;
}

inline double random_double() {
    //Random real in {0, 1}.
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    //Random real in [min, max]
    return min + (max-min)*random_double();
}

//Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif