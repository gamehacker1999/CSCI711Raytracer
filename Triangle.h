#pragma once
#include "Hittable.h"
class Triangle :
    public Hittable
{
	Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Material mat);
	~Triangle();

	bool Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record) override;
	float ComputeIntersection(const MyRay& ray, float& t);

protected:
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;

};

