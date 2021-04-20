#include "KDAccelerator.h"

//KDAccelerator::KDAccelerator(std::vector<Hittable> prims)
//{
//
//	BoundingBox rootVoxel;
//	rootVoxel.Center = Vector3(0, 0, 0);
//	rootVoxel.Extents = Vector3(10, 10, 10);
//	rootNode = GetNode(prims, rootVoxel);
//}
//
//void KDAccelerator::RayIntersect(MyRay& ray, HitRecord& record)
//{
//}
//
//pNode KDAccelerator::GetNode(std::vector<Hittable> prims, BoundingBox voxel)
//{
//	if (prims.size() < 10)
//	{
//		return pNode();
//	}
//
//	//find partition plane
//	Partition plane;
//	plane.split = SplittingPlane::XAxis;
//
//	Vector3 corners[8];
//	voxel.GetCorners(corners);
//
//	Vector3 min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
//	Vector3 max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
//
//	for (size_t i = 0; i < 8; i++)
//	{
//		if (corners[i].x < min.x)
//		{
//			min.x = corners[i].x;
//		}
//
//		if (corners[i].y < min.y)
//		{
//			min.y = corners[i].y;
//		}
//
//		if (corners[i].z < min.z)
//		{
//			min.z = corners[i].z;
//		}
//
//
//
//
//		if (corners[i].x > max.x)
//		{
//			max.x = corners[i].x;
//		}
//
//		if (corners[i].y > max.y)
//		{
//			max.y = corners[i].y;
//		}
//
//		if (corners[i].z > max.z)
//		{
//			max.z = corners[i].z;
//		}
//	}
//
//	Vector3 location = (max + min) / 2.0f;
//
//
//
//	//split the voxel based on the partition plane
//
//	//partition the elements
//
//	//return the left and right nodes
//
//}

bool KDNode::IsDone(int N, float minCV) const
{
	return false;
}

float KDNode::GetSplitPos()
{
	return 0.0f;
}

void KDNode::Build(KDNode node)
{
}

bool KDNode::Intersects(pHittable prim)
{
	return false;
}

void KDNode::IntersectAllTriangles(MyRay& ray, HitRecord& rec)
{
}

float KDNode::CalculateCost(const float& position)
{
	return 0.0f;
}

void KDNode::Traverse(MyRay& ray, HitRecord& rec)
{
}

void KDNode::SplitBox(const BoundingBox& voxel, const SplitPlane& plane, BoundingBox& vLeft, BoundingBox& vRight)
{
}

void KDNode::SurfaceAreaHeuristic(const SplitPlane& plane, const BoundingBox& voxel, int NL, int NR, float& CP, PlaneSide side) const
{
}

void KDNode::FindPlane(const std::vector<pHittable>& prims, const BoundingBox& voxel, int depth, SplitPlane& plane, float& cost, PlaneSide pSide) const
{
}

void KDNode::SortTriangles(const std::vector<pHittable>& prims, const SplitPlane& p, const PlaneSide& pside, std::vector<pHittable>& hL, std::vector<pHittable>& hR) const
{
}
typedef std::shared_ptr<KDNode> pNode;

pNode KDNode::RecBuild(std::vector<pHittable> prims, const BoundingBox& V, int depth, const SplitPlane& prevPlane)
{
	return pNode();
}
