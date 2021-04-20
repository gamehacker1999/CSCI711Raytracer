#pragma once
#include"Utils.h"

struct Material
{
	Color diffuseColor;
	Color specColor;
	float diffuseCeoff;
	float specCoeff;
	float specularity;
	float metalness;
	float roughness;
	float kr; //reflectivity
	float kt; //transmittability
	float ior; //index of refraction
};