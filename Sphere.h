#pragma once
#include "Hittable.h"
class Sphere :
	public Hittable
{
public:
	Sphere(Vector3 center, float radius, Material mat);
	~Sphere();

	bool Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record) override;

protected:
	Vector3 center;
	float radius;
};

