#include "Hittable.h"

Hittable::Hittable(Material mat)
{
	this->mat = mat;

	world = Matrix::Identity;
}

Hittable::~Hittable()
{
}
