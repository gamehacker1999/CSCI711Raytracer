#include "HittableManager.h"

HittableManager::HittableManager()
{
	size = 10000;
	count = 0;
}

HittableManager::~HittableManager()
{
	/*for (int i = 0; i < hittableList.size(); i++)
	{
		if (hittableList[i] != nullptr)
		{
			delete hittableList[i];
		}
	}

	hittableList.clear();
	hittableList.shrink_to_fit();*/

}


//this function checks if any hittable object has been hit by the ray
//it then checks which object is the closest to the ray
//if the closest object is found then save its parameter
bool HittableManager::ManageHits(const MyRay& ray, float tMin, float tMax, HitRecord& record, Vector3& color)
{
	HitRecord temp;
	bool hit = false;
	float closestParameter = tMax;

	for (int i = 0; i < count; i++)
	{
		if (hittableList[i]->Hit(ray, tMin, closestParameter, temp))
		{
			hit = true;

			if (temp.t < closestParameter)
			{
				closestParameter = temp.t;
				record = temp;
			}
		}
	}

	//light the closest point

	if (hit)
	{
		Vector3 diffuseColor = {};
		Vector3 specularColor = {};

		for (size_t i = 0; i < pointLights.size(); i++)
		{
			float shadowFactor = ShootShadowRays(record.point, pointLights[i]);

			if (shadowFactor!=0)
			{
				auto col1 = (pointLights[i].color * pointLights[i].intensity) * Lambertian(pointLights[i], record);
				diffuseColor += col1*shadowFactor;
				specularColor += (pointLights[i].color * pointLights[i].intensity) * PhongLighting(pointLights[i], record, ray)*shadowFactor;
			}
		}

		//Vector3 finalCol = (diffuseColor) + (specularColor);
		Vector3 finalCol = (record.material.diffuseCeoff * diffuseColor) + (record.material.specCoeff * specularColor);
		color = finalCol;
	}

	return hit;
}

float HittableManager::ShootShadowRays(Vector3 point, PointLight& light)
{
	Vector3 shadowRayDir = light.position - point;
	shadowRayDir.Normalize(shadowRayDir);
	MyRay shadowRay(point + 0.01 * shadowRayDir, shadowRayDir);
	HitRecord temp = {};

	bool hit = false;
	for (int i = 0; i < count; i++)
	{
		//temp = {};
		if (hittableList[i]->Hit(shadowRay, 0.0001, 100000, temp))
		{
			hit = true;
			//break;
		}
	}

	float shadowFactor = 1.f;

	if (hit)
	{
		shadowFactor = 0;

		if (temp.material.kt > 0)
		{
			shadowFactor += (temp.material.kt);
		}
	}
	else
	{
		shadowFactor == 1;
	}

	return shadowFactor;


}

void HittableManager::AddHittable(std::shared_ptr<Hittable> hittable)
{
	if (hittable != nullptr)
	{
		hittableList.emplace_back(hittable);
	}

	count++;
}

void HittableManager::AddPointLight(PointLight& light)
{
	pointLights.emplace_back(light);
}
