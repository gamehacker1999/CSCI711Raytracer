#pragma once
#include "Hittable.h"
#include <math.h>
class Torus :
    public Hittable
{
public:
	Torus(float sweepRadius, float tubeRadius, Vector3 pos, Material mat);
	~Torus();

	bool Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record) override;
	void LoadTorusMesh();
	bool ComputeNormal(const MyRay& ray, float& t);
	float ComputeIntersection(const MyRay& ray, float& t);

private:
	float sweepRad;
	Vector3 pos;
	float tubeRad;
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<Vector3> tangents;
	std::vector<Vector3> bitangents;
	//list of vertices and indices
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

};

