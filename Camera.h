#pragma once
#include"Utils.h"
#include "MyRay.h"

class Camera 
{
public:
    Camera(
        Vector3 lookfrom,
        Vector3 lookat,
        Vector3 vup,
        double aspect_ratio,
        double vfov = 90 // vertical field-of-view in degrees
    );

    MyRay GetRay(double s, double t) const;

private:
    Vector3 origin;
    Vector3 lower_left_corner;
    Vector3  horizontal;
    Vector3  vertical;
};