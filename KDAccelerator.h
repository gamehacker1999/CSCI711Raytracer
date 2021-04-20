#pragma once
#include<memory>
#include<vector>
#include"Hittable.h"
#include"MyRay.h"


enum SplittingAxis
{
	XAxis,
	YAxis,
	ZAxis
};
//
//struct Partition
//{
//	SplittingPlane split;
//	float location;
//};
//
//struct KDNode
//{
//	Partition partitioningPlane;
//	std::shared_ptr<KDNode> left;
//	std::shared_ptr<KDNode> right;
//	bool isLeaf;
//
//};
//
//typedef std::shared_ptr<KDNode> pNode;
//
//
//class KDAccelerator
//{
//public:
//	KDAccelerator(std::vector<Hittable> prims);
//	void RayIntersect(MyRay& ray, HitRecord& record);
//	pNode GetNode(std::vector<Hittable> prims, BoundingBox voxel);
//	
//	
//private:
//	pNode rootNode;
//};

struct SplitPlane
{
	SplitPlane(const SplittingAxis axis, const float pos):axis(axis), pos(pos) {};
	SplitPlane() {};

	SplittingAxis axis;
	float pos;

	bool operator==(const SplitPlane& sp) 
	{
		return(axis == sp.axis && pos == sp.pos);
	}

};


class KDNode
{
	typedef std::shared_ptr<KDNode> pNode;
	typedef std::shared_ptr<Hittable> pHittable;
public:

	KDNode() :leaf(false), prims(std::vector<pHittable>()) {};
	pNode left;
	pNode right;
	bool leaf;
	SplitPlane splitPlane;
	BoundingBox voxel;
	std::vector<pHittable> prims;

	bool IsDone(int N, float minCV) const;
	float GetSplitPos();
	void Build(KDNode node);
	bool Intersects(pHittable prim);
	void IntersectAllTriangles(MyRay& ray, HitRecord& rec);
	float CalculateCost(const float& position);
	void Traverse(MyRay& ray, HitRecord& rec);


	void SplitBox(const BoundingBox& voxel, const SplitPlane& plane, BoundingBox& vLeft, BoundingBox& vRight);

	typedef enum { LEFT = -1, RIGHT = 1, UNKNOWN = 0 } PlaneSide;

	void SurfaceAreaHeuristic(const SplitPlane& plane, const BoundingBox& voxel, int NL, int NR, float& CP, PlaneSide side) const;

	void FindPlane(const std::vector<pHittable>& prims, const BoundingBox& voxel, int depth, SplitPlane& plane,
		float& cost, PlaneSide pSide) const;

	void SortTriangles(const std::vector<pHittable>& prims, const SplitPlane& p, const PlaneSide& pside, 
		std::vector<pHittable>& hL, std::vector<pHittable>& hR) const;

	pNode RecBuild(std::vector<pHittable> prims, const BoundingBox& V, int depth, const SplitPlane& prevPlane);


};

