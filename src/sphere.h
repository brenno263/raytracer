#pragma once

#include <cmath>

#include "vec3.h"
#include "ray.h"
#include "shape.h"

class Sphere : public Shape
{
private:
	Vec3 center;
	double radius;

public:
    Sphere(Vec3 center, double radius, std::shared_ptr<Material> mat)
    {
        this->center = center;
        this->radius = radius;
        this->mat = mat;
    }

    bool getCollision(const Ray& ray, double min, double max, Collision& out) const override
    {
        //t^2(D⋅D) + 2t(D⋅(O−C))+(O−C)⋅(O−C)−r^2=0

        Vec3 center_to_ray_origin = ray.origin() - center;
        double a = ray.direction().dot(ray.direction());
        double half_b = ray.direction().dot(center_to_ray_origin);
        double c = center_to_ray_origin.dot(center_to_ray_origin) - (radius * radius);

        double discriminant = half_b * half_b - a * c;
        if(discriminant < 0) return false;

        double sqrtd = sqrt(discriminant);

        double root = (-half_b - sqrtd) / a;
        if(root < min || root > max) {
            root = (-half_b + sqrtd) / a;
            if (root < min || root > max)
                return false;
        }

        out.t = root;
        out.point = ray.destination(root);
        out.setFaceNormal(ray, (out.point - center) / radius);
        out.mat = mat;

        return true;
    };
};
