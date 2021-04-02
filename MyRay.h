#ifndef __MYRAY_H__
#define __MYRAY_H__
#include"Utils.h"
class MyRay
{
public:
	 MyRay();
	 MyRay(Vector3 a, Vector3 b);
	 Vector3 GetOrigin() const;
	 Vector3 GetDirection() const;
	 Vector3 GetPointAtParameter(float t) const;
	 void TransformRayToLocalSpace(Matrix world);
	 ~MyRay();

	Vector3 a;
	Vector3 b;
};

#endif // !__RAY_H__




