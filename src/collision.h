#pragma once

class Material;

struct Collision {
    Vec3 point;
    Vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<Material> mat;

    void setFaceNormal(const Ray& ray, const Vec3& outward_normal)
    {
        front_face = ray.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : outward_normal * -1;
    }
};