#pragma once

#include <vector>
#include "vec3.h"



class Shape
{
private:

public:
    std::shared_ptr<Material> mat;
    virtual bool getCollision(const Ray& ray, double min, double max, Collision& out) const = 0;
};

class ShapeList : public Shape
{
private:

public:
    std::vector<std::shared_ptr<Shape>> shapes;

    ShapeList() {};

    void add(std::shared_ptr<Shape> s)
    {
        shapes.push_back(s);
    }

    bool getCollision(const Ray& ray, double min, double max, Collision& out) const override
    {
        Collision coll;
        bool hit_anything = false;
        auto closest_so_far = max;

        for (const auto& s : shapes) {
            if (s->getCollision(ray, min, closest_so_far, coll)) {
                hit_anything = true;
                closest_so_far = coll.t;
                out = coll;
            }
        }

        return hit_anything;
    }
};

