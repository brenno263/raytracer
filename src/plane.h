#pragma once

class Plane : public Shape {
protected:
    Vec3 point;
    Vec3 normal;

public:
    Plane(const Vec3 &point, const Vec3 &normal, std::shared_ptr<Material> mat) {
        this->point = point;
        this->normal = normal.normalized();
        this->mat = mat;
    }

    bool getCollision(const Ray &ray, double min, double max, Collision &coll) const override {
        double denom = ray.direction().dot(normal) * -1;
        if (f_equals(denom, 0)) return false;

        double numer = (ray.origin() - point).dot(normal);

        double t = numer / denom;

        coll.t = t;
        coll.point = ray.destination(t);
        coll.setFaceNormal(ray, normal);
        coll.mat = mat;

        if (t < min || t > max) return false;

        return true;
    }
};
