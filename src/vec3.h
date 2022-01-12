#pragma once

#include <iostream>
#include <cmath>

#define MATERIAL_DIFFUSE 0
#define MATERIAL_SPECULAR 1

class Vec3 {
private:
    double v[3];

public:
    //static Vec3 zero;
    //static Vec3 one;
    //Vec3 right;
    //Vec3 down;
    //Vec3 forwards;
    //Vec3 zero = Vec3(0, 0, 0);
    //Vec3 one = Vec3(1, 1, 1);

    double x() const { return v[0]; }

    double y() const { return v[1]; }

    double z() const { return v[2]; }

    Vec3() {
        v[0] = 0;
        v[1] = 0;
        v[2] = 0;
    }

    Vec3(double x, double y, double z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    double magnitude() const {
        return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }

    Vec3 operator+(const Vec3 &other) const {
        return Vec3(v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2]);
    }

    Vec3 operator-(const Vec3 &other) const {
        return Vec3(v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2]);
    }

    Vec3 operator*(double s) const {
        return Vec3(v[0] * s, v[1] * s, v[2] * s);
    }

    Vec3 operator/(double s) const {
        s = 1 / s;
        return Vec3(v[0] * s, v[1] * s, v[2] * s);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vec3 &vec3) {
        os << (int) (vec3.x() * 255) << " " << (int) (vec3.y() * 255) << " " << (int) (vec3.z() * 255);
        return os;
    }

    double dot(const Vec3 &in) const {
        return v[0] * in.v[0] + v[1] * in.v[1] + v[2] * in.v[2];
    }

    Vec3 cross(const Vec3 &in) const {
        return Vec3(
                v[1] * in.v[2] - v[2] * in.v[1],
                v[2] * in.v[0] - v[0] * in.v[2],
                v[0] * in.v[1] - v[1] * in.v[0]
        );
    }

    Vec3 attenuate(const Vec3 &in) const {
        return Vec3(
            v[0] * in.v[0],
            v[1] * in.v[1],
            v[2] * in.v[2]
        );
    }

    Vec3 normalized() const {
        double inverse_length = 1 / this->magnitude();
        return Vec3(
                v[0] * inverse_length,
                v[1] * inverse_length,
                v[2] * inverse_length
        );
    }

    bool nearZero() const {
        return fabs(v[0]) < EPSILON && fabs(v[1]) < EPSILON && fabs(v[2]) < EPSILON;
    }

    void print() {
        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
    }

    static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }

    static Vec3 random(double min, double max) {
        double delta = max - min;
        return Vec3(
                random_double() * delta + min,
                random_double() * delta + min,
                random_double() * delta + min
        );
    }

    static Vec3 randomInUnitSphere() {
        Vec3 attempt = Vec3::random(-1, 1);
        while (attempt.dot(attempt) > 1) {
            attempt = Vec3::random(-1, 1);
        }
        return attempt;
    }

    static Vec3 randomOnUnitSphere() {
        return randomInUnitSphere().normalized();
    }
};
