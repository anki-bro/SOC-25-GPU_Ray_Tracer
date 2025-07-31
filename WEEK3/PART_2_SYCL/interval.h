#ifndef INTERVAL_H
#define INTERVAL_H
#include "config.h"

class interval {
    public:
        float min; // min value of the interval
        float max; // max value of the interval
        interval() : min(+infinity), max(-infinity) {}
        interval(float min_val, float max_val) : min(min_val), max(max_val) {}

        float length() const {
            return max - min;
        }
    
        bool contains(float value) const {
            return value >= min && value <= max;
        }

        bool surrounds(float value) const {
            return value > min && value < max;
        }
        
        static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity); // Represents an empty interval
const interval interval::universe = interval(-infinity, +infinity); // Represents the entire range of float values

#endif // INTERVAL_H