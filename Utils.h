#pragma once
#define NOMINMAX 
#include <iostream>
#include <fstream>
#include<random>
#include<d3d12.h>
#include<DirectXMath.h>
#include<memory>
#include<SimpleMath.h>
#include<wrl\client.h>
#include<stdexcept>
#include<string>
#include "happly.h"


#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;

using namespace DirectX;
using namespace DirectX::SimpleMath;

//Vector3 l = (pointLights[i].position - record.point);
//
//Vector3 f0 = Vector3(0.04, 0.04, 0.04);
//auto col = record.material.diffuseColor;
//f0 = Vector3::Lerp(f0, Vector3(col.R(),col.G(),col.B()), record.material.metalness);
//
//l.Normalize();
//Vector3 F;
//auto col2 = (pointLights[i].color * pointLights[i].intensity) * CookTorrence(record.normal, record.material.roughness, -ray.GetDirection(),
//	f0, l, F);
//
//
//Vector3 ks = F;
//Vector3 kd = Vector3(1.0f, 1.0f, 1.0f) - ks;
//kd *= (Vector3(1.0f, 1.0f, 1.0f) - Vector3(record.material.metalness, record.material.metalness, record.material.metalness));
//
//diffuseColor += col1 * kd;
//specularColor += col2*ks;

struct Vertex
{
	Vector3 Position;	    // The position of the vertex
	//DirectX::XMFLOAT4 Color;        // The color of the vertex
	Vector3 normal; //normal of the current vertex
	Vector3 tangent;
	Vector2 uv; //texture coordinates of the vertex in u v space
};

inline float modulo(const float& x)
{
	return x - std::floor(x);
}

inline float smoothstep(float edge0, float edge1, float x) {
    // Scale, bias and saturate x to 0..1 range
    x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    // Evaluate polynomial
    return x * x * (3 - 2 * x);
}

inline float mandelbrot(Vector2& c)
{
#if 1
    {
        float c2 = c.Dot(c);
        // skip computation inside M1 - http://iquilezles.org/www/articles/mset_1bulb/mset1bulb.htm
        if (256.0 * c2 * c2 - 96.0 * c2 + 32.0 * c.x - 3.0 < 0.0) return 0.0;
        // skip computation inside M2 - http://iquilezles.org/www/articles/mset_2bulb/mset2bulb.htm
        if (16.0 * (c2 + 2.0 * c.x + 1.0) - 1.0 < 0.0) return 0.0;
    }
#endif


    const float B = 256.0;
    float l = 0.0;
    Vector2 z = Vector2(0.0);
    for (int i = 0; i < 512; i++)
    {
        z = Vector2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (z.Dot(z) > (B * B)) break;
        l += 1.0;
    }

    if (l > 511.0) return 0.0;

    // ------------------------------------------------------
    // smooth interation count
    //float sl = l - log(log(length(z))/log(B))/log(2.0);

    // equivalent optimized smooth interation count
    float sl = l - log2(log2(z.Dot(z))) + 4.0;

    //float al = smoothstep(-0.1, 0.0, sin(0.5 * 6.2831 * iTime));
    //l = std::(l, sl, al);

    return l;
}

inline float NextRand(UINT& s)
{
    s = (1664525u * s + 1013904223u);
    return float(s & 0x00FFFFFF) / float(0x01000000);
}

// Generates a seed for a random number generator from 2 inputs plus a backoff
inline UINT InitSeed(UINT val0, UINT val1, UINT backoff = 16)
{
    UINT v0 = val0, v1 = val1, s0 = 0;

    for (UINT n = 0; n < backoff; n++)
    {
        s0 += 0x9e3779b9;
        v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
        v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
    }
    return v0;
}

inline Vector3 CalculateDiffuseDirection(float u1, float u2, Vector3 N)
{
    float cosTheta = pow(u1, 1.0f / (2.f));
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
    float phi = 2 * 3.1415926 * u2;
    Vector3 direction = Vector3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);

    Vector3 up = abs(N.x) < 0.99 ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(0.0f, 0.0f, 1.0f); //up vector is based on the value of N
    Vector3 tangent = N.Cross(up);
    tangent.Normalize();
    Vector3 bitangent = N.Cross(tangent);
    bitangent.Normalize();

    Vector3 sampleVec = tangent * direction.x + bitangent * direction.y + direction.z * N;

    sampleVec.Normalize();

    return sampleVec;

}

inline Vector3 CalculateSpecularDirection(float u1, float u2, Vector3 N, float specularity)
{
    float cosTheta = pow(u1, 1.0f / (specularity + 1.0f));
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
    float phi = 2 * 3.1415926 * u2;
    Vector3 direction = Vector3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);

    Vector3 up = abs(N.x) < 0.99 ? Vector3(1.0f, 0.0f, 0.0f) : Vector3(0.0f, 0.0f, 1.0f); //up vector is based on the value of N
    Vector3 tangent = N.Cross(up);
    tangent.Normalize();
    Vector3 bitangent = N.Cross(tangent);
    bitangent.Normalize();

    Vector3 sampleVec = tangent * direction.x + bitangent * direction.y + direction.z * N;

    sampleVec.Normalize();

    return sampleVec;

}