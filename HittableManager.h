#ifndef __HITTABLE_MANAGER_H__
#define __HITTABLE_MANAGER_H__
#include <vector>
#include"Hittable.h"
#include "Lights.h"
#include<memory>
class HittableManager
{
public:
	HittableManager();
	~HittableManager();

	bool ManageHits(const MyRay& ray, float tMin, float tMax, HitRecord& record, Vector3& color);
	float ShootShadowRays(Vector3 point, PointLight& light);

	void AddHittable(std::shared_ptr<Hittable> hittable);
	void AddPointLight(PointLight& light);

private:
	std::vector<std::shared_ptr<Hittable>> hittableList;
	std::vector<PointLight> pointLights;
	//Hittable** hittableList;
	int size;
	int count;
};
#endif // !__HITTABLE_MANAGER_H__


