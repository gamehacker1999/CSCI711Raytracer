#include "Camera.h"

Camera::Camera(
    Vector3 lookfrom,
    Vector3 lookat,
    Vector3 vup,
    double aspect_ratio,
    double vfov  // vertical field-of-view in degrees
)
{
    auto theta = XMConvertToRadians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;   

    Vector3 worldUp(0.0f, 1.0f, 0.0f);
    auto w = (lookat - lookfrom);
    w.Normalize();
    XMVECTOR right = XMVector3Cross(XMLoadFloat3(&worldUp), XMLoadFloat3(&w)); //finding the right vector
    XMStoreFloat3(&vup, XMVector3Cross(XMLoadFloat3(&w), right)); //finding the up vector

    origin = lookfrom;
    horizontal = viewport_width * right;
    vertical = viewport_height * vup;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 + w;
}

MyRay Camera::GetRay(double s, double t) const
{
    return MyRay(origin, lower_left_corner + s * horizontal + t * vertical - origin);
}