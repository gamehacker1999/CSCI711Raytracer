#ifndef __HITTABLE_H__
#define __HITTABLE_H__
#include"Utils.h"
#include"MyRay.h"
#include "Material.h"

//class Material;

struct HitRecord
{
	float t; //parameter of hit
	Vector3 point; //point at that parameter
	Vector3 normal; //surface normal
	Material material;
	int depth;
};

//class that defines an object that can be hit by a ray
class Hittable
{
public:
	Hittable(Material mat);
	virtual ~Hittable();

	virtual bool Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record) = 0;

protected:
	Matrix world;
	BoundingSphere boundingSphere;
	Material mat;
};

#endif // !__HITTABLE_H__