//
// Created by LEI XU on 5/16/19.
//

#ifndef RAYTRACING_INTERSECTION_H
#define RAYTRACING_INTERSECTION_H

#include "vector.hpp"
#include "material.hpp"

class Object;
class Sphere;

struct Intersection
{
    Intersection()
    {
        happened = false;
        coords = Vector3f();
        normal = Vector3f();
        distance = std::numeric_limits<double>::max();
        obj = nullptr;
        m = nullptr;
    }
    bool happened;
    Vector3f coords;
    Vector3f normal;
    double distance;
    Object *obj;
    Material *m;

    friend std::ostream &operator<<(std::ostream &os, const Intersection &is)
    {
        os << "Intersection(" << is.happened << ", " << is.coords << ", " << is.normal << ", " << is.distance << ", " << is.obj << ", " << is.m << ")";
        return os;
    }
};
#endif // RAYTRACING_INTERSECTION_H
