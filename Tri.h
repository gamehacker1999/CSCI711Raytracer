#pragma once
#include "Hittable.h"
class Tri :
    public Hittable
{
public:
	Tri(Vector3 v1, Vector3 v2, Vector3 v3, Material mat);
	~Tri();

	bool Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record) override;
	float ComputeIntersection(const MyRay& ray, float& t);

protected:
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;

};

