#ifndef INTERVAL_H
#define INTERVAL_H

class Interval {            // Interval class
    public:
        double min, max;    // Minimum and maximum values

        Interval() : min(+infinity), max(-infinity) {}

        Interval(double _min, double _max) : min(_min), max(_max) {}

        // Check if interval contains a value. Can be min, max, or in between
        bool contains(double x) const {
            return min <= x && x <= max;
        }

        // Check if interval surrounds a value. Must be in between min and max
        bool surrounds(double x) const {
            return min < x && x < max;
        }

        // Collapse value to the nearest bound
        double clamp(double x) const {
            return std::max(min, std::min(max, x));
        }

        static const Interval empty, universe;
};

const static Interval empty(+infinity, -infinity);
const static Interval universe(-infinity, +infinity);

#endif