#include "MyRay.h"

MyRay::MyRay()
{
}

MyRay::MyRay(Vector3 a, Vector3 b)
{
	this->a = a;
	this->b = b;
	this->b.Normalize();
}

Vector3 MyRay::GetOrigin() const
{
	return a;
}

Vector3 MyRay::GetDirection() const
{
	return b;
}

Vector3 MyRay::GetPointAtParameter(float t) const
{
	return a + t * b;
}

void MyRay::TransformRayToLocalSpace(Matrix world)
{
	//transforming the ray to the local space of an object
	world = world.Invert();
	a = Vector3::Transform(a, world);
	b = Vector3::TransformNormal(b, world);
}

MyRay::~MyRay()
{
}
