#pragma once

#include "vec3.h"

class Ray {
private:
    Vec3 o, d;

public:
    Ray() {}

    Ray(const Vec3 &origin, const Vec3 &direction) {
        o = origin;
        d = direction;
    }

    Vec3 origin() const {
        return o;
    }

    Vec3 direction() const {
        return d;
    }

    Vec3 destination(double t) const {
        return o + (d * t);
    }

};