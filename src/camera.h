#pragma once
#include "vec3.h"

class Camera
{
public:
    Camera(const Vec3& position, const Vec3& direction, const Vec3& up, double fov, double aspect_r)
    {
        Vec3 x, y, z;
        z = direction.normalized();
        y = up.normalized();
        x = z.cross(y);
        this->position = position;
        this->vert = y * (double)(2 * tan(fov / 2));
        this->hoz = x * (double)(-2 * tan(fov / 2) * aspect_r);
        this->ul = position + z - (hoz * 0.5f) + (vert * 0.5f);
    }

    // x, y in [0, 1), return not normalized
    Ray getRay(double x, double y)
    {
        return Ray(position, ((ul + (hoz * x) - (vert * y) - position).normalized()));
    }

private:
    //self-explanatory
	Vec3 position;
    //relative position of the upper left corner of the viewport
    Vec3 ul;
    //vector from left to right viewport borders
    Vec3 hoz;
    //vector from bottom to top viewport borders
    Vec3 vert;
};