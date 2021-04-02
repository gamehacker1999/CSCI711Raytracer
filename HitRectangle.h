#pragma once
#include "Hittable.h"
class HitRectangle :
    public Hittable
{
public:
	HitRectangle(Vector3 position, Vector3 normal, Vector3 size, Material mat);
	~HitRectangle();

	bool Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record) override;

	bool RayPlaneIntersect(const MyRay& ray, float& t);
	bool RayRectIntersect(const MyRay& ray, float& t);

protected:
	Vector3 pos;
	Vector3 normal;
	Vector3 size;
	float halfX;
	float halfY;
	Vector3 dirx;
	Vector3 diry;
	Vector4 plane;

	Matrix worldMat;

	//Texture variables
	int width;
	int height;
	int comp;
	unsigned char* data;
	int bytesPerLine;

};

