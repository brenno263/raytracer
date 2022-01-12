#pragma once

class Triangle : public Plane{

private:
    Vec3 a, b, c;

public:
    Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, std::shared_ptr<Material> mat)
    : Plane(a, (b - a).cross(c - a), mat)
    {
        this->a = a;
        this->b = b;
        this->c = c;

        this->mat = mat;
    }

    Vec3 getBarymetric(Vec3 p) const
    {
        double n_squared_inv = 1 / normal.dot(normal);

        return Vec3(
                normal.dot((b - p).cross(c - p)) * n_squared_inv,
                normal.dot((c - p).cross(a - p)) * n_squared_inv,
                normal.dot((a - p).cross(b - p)) * n_squared_inv
        );
    }

    bool inTriangle(Vec3 p) const
    {
        Vec3 bary = getBarymetric(p);

        return bary.x() >= 0 && bary.y() >= 0 && bary.z() >= 0;
    }

    bool getCollision(const Ray& ray, double min, double max, Collision& coll) const override
    {
        if(Plane::getCollision(ray, min, max, coll))
        {
            if(coll.t < min || coll.t > max) return false;
            return inTriangle(coll.point);
        }

        return false;
    }
};

//make me later
class Quad : public Plane {

};

