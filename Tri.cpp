#include "Tri.h"

Tri::Tri(Vector3 v1, Vector3 v2, Vector3 v3, Material mat): Hittable(mat)
{
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
}

Tri::~Tri()
{
}

bool Tri::Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record)
{

	float t = ComputeIntersection(ray, record.t);

	if (t > 0)
	{
		record.t = t;
		record.point = ray.GetPointAtParameter(record.t);
		record.material = mat;
		return true;
	}

	return false;
}

float Tri::ComputeIntersection(const MyRay& ray, float& t)
{

	auto tempRay = ray;
	tempRay.TransformRayToLocalSpace(world);
	Ray newRay;
	newRay.direction = tempRay.GetDirection();
	newRay.position = tempRay.GetOrigin();
	float newDist = t;

	if (newRay.Intersects(v1, v2, v3, newDist))
	{

		if (newDist < t)
		{
			t = newDist;
		}
	}

	return t;
}
