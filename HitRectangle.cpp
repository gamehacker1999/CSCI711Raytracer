#include "HitRectangle.h"
#include <random>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#define CHECKER_SIZE 1
#define BRICK_WIDTH 0.48
#define BRICK_HEIGHT 0.08
#define MORTAR_THICKNESS 0.01
#define U_SCALE 10
#define V_SCALE 10

#define BMHEIGHT BRICK_HEIGHT+MORTAR_THICKNESS
#define BMWIDTH BRICK_WIDTH+MORTAR_THICKNESS

struct Brick
{
	Color mortalColor = Color(.5, 0.5, 0.5);
	Color brickColor = Color(0.5, 0.15, 0.1);
};


HitRectangle::HitRectangle(Vector3 position, Vector3 normal, Vector3 size, Material mat) :Hittable(mat)
{
	this->pos = position;
	this->normal = normal;
	this->size = size;

	halfX = 0.5 * size.x;
	halfY = 0.5 * size.z;

	dirx = Vector3(1, 0, 0);
	diry = Vector3(0, 0, 1);

	plane = Vector4(normal.x, normal.y, normal.z, -normal.Dot(position));

	Matrix scaleMat = Matrix::CreateScale(size);
	Matrix transMat = Matrix::CreateTranslation(pos);

	worldMat = scaleMat * transMat;

	worldMat.Invert(worldMat);
	data = stbi_load("Textures/RadialGradientNorm.PNG", &width, &height, &comp, 4);

	bytesPerLine = width * 4;

}

HitRectangle::~HitRectangle()
{
}

bool HitRectangle::Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record)
{

	if (RayRectIntersect(ray, record.t))
	{
		Vector3 e1 = -this->normal.Cross(Vector3(1, 0, 0));
		e1.Normalize(e1);

		Vector3 e2 = -this->normal.Cross(e1);
		e2.Normalize(e2);

		Vector3 point = ray.GetPointAtParameter(record.t);

		float u = e1.Dot(point);
		float v = e2.Dot(point);

		// Pick a direction for our texture's vertical "v" axis. 
		// Default for floors/ceilings:
		Vector3 vDirection = Vector3(0, 0, 1);

		// Get the perpendicular in-plane vector to use as our "u" direction.
		Vector3 uDirection = normal.Cross(vDirection);

		Vector3 localPoint = Vector3::Transform(point, worldMat);

		u = (localPoint.x + 1) / 2.0f;
		v = (localPoint.z + 1) / 2.0f;

		u *= U_SCALE;
		v *= V_SCALE;

		//Color c;
		////image texture
		//{
		//	u = u - int(u);
		//	v = v - int(v);
		//	auto i = static_cast<int>(u * width);
		//	auto j = static_cast<int>(v * height);
		//
		//	// Clamp integer mapping, since actual coordinates should be less than 1.0
		//	if (i >= width)  i = width - 1;
		//	if (j >= height) j = height - 1;
		//
		//	const auto color_scale = 1.0 / 255.0;
		//	//auto pixel = data + j * bytesPerLine + i * 4;
		//	//
		//	//int index = (v * width + u) * 4;
		//	//
		//	//float x = data[index];
		//	//float y = data[index + 1];
		//	//float z = data[index + 2];
		//	//float a = data[index + 3];
		//	//
		//	//c = Color(x, y, z, a);
		//
		//	auto pixel = data + j * bytesPerLine + i * 4;
		//
		//	c = Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2], 1.0f);
		//
		//
		//	this->mat.diffuseColor = c;
		//}

		//checker board
		Color color;
		{
			int pattern = (modulo(v * CHECKER_SIZE) < 0.5) ^ (modulo(u * CHECKER_SIZE) < 0.5);
			float r = static_cast <float> (rand())*4 / static_cast <float> (RAND_MAX);
		
			color = Color(1, 1, 0, 1);

			if (pattern)
			{
				color = Color(1, 0, 0, 1);
			}	
		
			this->mat.diffuseColor = color;
		}
		
		//Brick
		//Color Ct;
		//{
		//	Brick brick;
		//	float row = v / BMHEIGHT;
		//	float col = u / BMWIDTH;
		//	if (std::fmodf(row, 2) == 0)
		//		u += (BMWIDTH / 2);
		//
		//	u = std::fmodf(u, BRICK_WIDTH);
		//	v = std::fmodf(v, BRICK_HEIGHT);
		//
		//	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//
		//	r = r * 2.0 - 1.0;
		//	Ct = Color(brick.brickColor.R() + (r * 0.17),
		//		brick.brickColor.G() + (r * 0.17),
		//		brick.brickColor.B() + (r * 0.17));
		//
		//	Vector3 returnNormal = this->normal;
		//	if (v < MORTAR_THICKNESS || u < MORTAR_THICKNESS)
		//	{
		//		Ct = brick.mortalColor;
		//
		//	}
		//	this->mat.diffuseColor = Ct;
		//}

		//Color mCol;
		//
		//Vector2 uv = Vector2(u*100, v*100);
		//float l = mandelbrot(uv);
		//
		//float a = 3.0 + l * 0.15;
		//
		//float cx = cos(0.0 + a);
		//float cy = cos(0.6 + a);
		//float cz = cos(1.0 + a);
		//
		//mCol += 0.5 * Color(cx,cy,cz);
	
		//this->mat.diffuseColor = mCol;
		record.material = mat;
		record.t = record.t;
		record.point = point;
		record.normal = this->normal;
		return true;
	}

	return false;
}

bool HitRectangle::RayPlaneIntersect(const MyRay& ray, float& t)
{
	t = -(plane.Dot(Vector4(ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, 1.0f)) / Vector3(plane).Dot(ray.GetDirection()));
		return t > 0.0;
}

bool HitRectangle::RayRectIntersect(const MyRay& ray, float& t)
{
	bool intersect = RayPlaneIntersect(ray, t);
	if (intersect)
	{
		Vector3 position = ray.GetOrigin() + ray.GetDirection() * t;
		Vector3 lpos = position - pos;

		float x = lpos.Dot(dirx);
		float y = lpos.Dot(diry);

		if (abs(x) > halfX || abs(y) > halfY)
			intersect = false;
	}

	return intersect;
}
