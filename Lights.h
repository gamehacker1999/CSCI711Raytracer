#pragma once
#define NOMINMAX
#include"Utils.h"
#include <algorithm>
struct PointLight
{
	Vector3 position;
	float intensity;
	Vector3 color;
};

inline float saturate(float value)
{
	return std::clamp<float>(value, 0.0f, 1.0f);
}

inline Vector3 Lambertian(PointLight& light, HitRecord& rec)
{
	Vector3 lightDir = rec.point - light.position;

	Vector3 L = -lightDir;

	L.Normalize(L);

	Vector3 N;

	rec.normal.Normalize(N);

	float ndotl = std::clamp<float>(N.Dot(L), 0.0f, 1.0f);

	Vector3 diffCol = Vector3(rec.material.diffuseColor.R(), rec.material.diffuseColor.G(), rec.material.diffuseColor.B());
	Vector3 col = diffCol * ndotl;

	return col;

}

inline Vector3 PhongLighting(PointLight& light, HitRecord& rec, const MyRay& ray)
{
	Vector3 lightDir = rec.point - light.position;
	Vector3 L = -lightDir;
	L.Normalize(L);

	Vector3 N;
	rec.normal.Normalize(N);

	Vector3 V = ray.GetOrigin() - rec.point;

	V.Normalize(V);

	Vector3 R;
	Vector3::Reflect(-L, N, R);

	Vector3 specColor = Vector3(rec.material.specColor.R(), rec.material.specColor.G(), rec.material.specColor.B());

	float rdotv = std::clamp<float>(R.Dot(V), 0.0f, 1.0f);

	rdotv = pow<float>(rdotv, rec.material.specularity);

	return Vector3(specColor * rdotv);

}

inline Vector3 BlinnPhongLighting(PointLight& light, HitRecord& rec, const MyRay& ray)
{
	Vector3 lightDir = rec.point - light.position;
	Vector3 L = -lightDir;
	L.Normalize(L);

	Vector3 N;
	rec.normal.Normalize(N);

	Vector3 V = ray.GetOrigin() - rec.point;

	V.Normalize(V);

	Vector3 H = V + L;

	H.Normalize(H);

	Vector3 specColor = Vector3(rec.material.specColor.R(), rec.material.specColor.G(), rec.material.specColor.B());

	float rdotv = std::clamp<float>(H.Dot(N), 0.0f, 1.0f);

	rdotv = pow<float>(rdotv, rec.material.specularity);

	return Vector3(specColor * rdotv);

}

//function for the fresnel term(Schlick approximation)
inline Vector3 Fresnel(Vector3 h, Vector3 v, Vector3 f0)
{
	//calculating v.h
	float VdotH = saturate(v.Dot(h));
	//raising it to fifth power
	float VdotH5 = pow(1 - VdotH, 5);

	Vector3 finalValue = f0 + (Vector3(1-f0.x,1-f0.y,1-f0.z)) * VdotH5;

	return finalValue;
}

//fresnel shchlick that takes the roughness into account
inline Vector3 FresnelRoughness(float NdotV, Vector3 f0, float roughness)
{
	float VdotH = saturate(NdotV);

	float VdotH5 = pow(1 - VdotH, 5);


	Vector3 finalValue = f0 + (Vector3::Max(Vector3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), f0) - f0) * VdotH5;

	return finalValue;

}

//function for the Geometric shadowing
// k is remapped to a / 2 (a is roughness^2)
// roughness remapped to (r+1)/2
inline float GeometricShadowing(
	Vector3 n, Vector3 v, Vector3 h, float roughness)
{
	// End result of remapping:
	float k = pow(roughness + 1, 2) / 8.0f;
	float NdotV = saturate(n.Dot(v));

	// Final value
	return NdotV / (NdotV * (1 - k) + k);
}


//function for the GGX normal distribution of microfacets
inline float SpecularDistribution(float roughness, Vector3 h, Vector3 n)
{
	//remapping the roughness
	float a = pow(roughness, 2);
	float a2 = a * a;

	float NdotHSquared = saturate(n.Dot(h));
	NdotHSquared *= NdotHSquared;

	float denom = NdotHSquared * (a2 - 1) + 1;
	denom *= denom;
	denom *= XM_PI;

	return a2 / denom;

}


//function that calculates the cook torrence brdf
inline Vector3 CookTorrence(Vector3 n, float roughness, Vector3 v, Vector3 &f0, Vector3 l, Vector3& F)
{

	Vector3 h = (n + l);
	h.Normalize();

	float D = SpecularDistribution(roughness, h, n);
	F = Fresnel(h, v, f0);
	float G = GeometricShadowing(n, v, h, roughness) * GeometricShadowing(n, l, h, roughness);

	Vector3 numSpec = D * F * G;
	float denomSpec = 4.0f * (std::max)(n.Dot(v), 0.0f) * (std::max)(n.Dot(l), 0.0f);
	Vector3 specular = numSpec / std::max(denomSpec, 0.0001f); //just in case denominator is zero

	return specular;

}