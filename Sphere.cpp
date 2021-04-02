#include "Sphere.h"

Sphere::Sphere(Vector3 center, float radius, Material mat) :Hittable(mat)
{
	this->center = center;
	this->radius = radius;
}

Sphere::~Sphere()
{

}

bool Sphere::Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record)
{
	// this is a quadratic equation of the sphere where t is the solution of the intersection
	float a =  ray.GetDirection().Dot(ray.GetDirection());
	Vector3 originToCenter = center-ray.GetOrigin();
	float c = originToCenter.Dot(originToCenter) - (radius * radius);
	//float b = ray.GetDirection().Dot(originToCenter);
	auto rayOrigin = ray.GetOrigin();
	auto rayDir = ray.GetDirection();
	float b = 2 * (rayDir.x * (rayOrigin.x - center.x) + rayDir.y * (rayOrigin.y - center.y) + rayDir.z * (rayOrigin.z - center.z));

	float discriminant = (b * b) - (4 * c);

	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / 2;
		if (temp > tMin && temp < tMax)
		{
			record.t = temp;
			record.point = ray.GetPointAtParameter(record.t);
			record.normal = (record.point - center);
			record.normal.Normalize();
			record.material = mat;
			return true;
		}
		
		temp = (-b + sqrt(discriminant)) / 2;
		if (temp > tMin && temp < tMax)
		{
			record.t = temp;
			record.point = ray.GetPointAtParameter(record.t);
			record.normal = (record.point - center);
			record.normal.Normalize();
			record.material = mat;
			return true;
		}

	}

	return false;
}
