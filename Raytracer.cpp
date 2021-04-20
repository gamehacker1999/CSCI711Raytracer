// Raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include"Utils.h"
#include "Camera.h"
#include"Sphere.h"
#include "HitRectangle.h"
#include"Torus.h"
#include"Tri.h"
#include"HittableManager.h"
#include<thread>
#include<future>
#include<atomic>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAX_DEPTH 8
#define NUM_SAMPLES 1.f

Color ToneReproductionReinhardt(Vector3 hdrColor, float keyValue, float a = 0.18)
{

    float rs = (a / keyValue) * hdrColor.x;
    float gs = (a / keyValue) * hdrColor.y;
    float bs = (a / keyValue) * hdrColor.z;

    Vector3 newCol = Vector3(rs, gs, bs);

    // reinhard tone mapping
    Vector3 mapped = newCol / (newCol + Vector3(1.0, 1.0f, 1.0f));
    Color col = Color(mapped.x, mapped.y, mapped.z, 1.0);

    return col;
}

Color ToneReproductionWard(Vector3 hdrColor, float keyValue, float ldMax)
{
    
    float num = (1.219f + std::pow(ldMax / 2.f, 0.4));
    float denom = (1.219 + std::pow(keyValue, 0.4));

    float sf = num / denom;

    sf = std::pow(sf, 2.5f);

    Vector3 mapped = (hdrColor * sf)/ldMax;

    Color col = Color(mapped.x, mapped.y, mapped.z, 1.0);

    return col;
}

Color ToneReproductionAdaptiveLogarithmic(Vector3 hdrColor, float bias, 
    float maxLuma, float curLuma, float ldMax)
{

    float term1Num = (ldMax * 0.01);
    float term1Denom = std::log10(maxLuma + 1);

    float term1 = term1Num / term1Denom;

    float term2Num = std::log(curLuma + 1);
    float term2Denom = std::log(2 + (std::pow(curLuma / maxLuma, (std::log(bias) / std::log(0.5))))*0.8);

    float term2 = term2Num / term2Denom;

    float ld = term1 * term2;

    Vector3 mapped = (hdrColor * ld) / ldMax;

    Color col = Color(mapped.x, mapped.y, mapped.z, 1.0);

    return col;
}

float CalcLuminance(Color color)
{
    return (color.R() * 0.27f + color.G() * 0.67 + 0.06 * color.B());
}

Color RayColor(const MyRay& r, HittableManager& world, int depth = 0)
{

    HitRecord rec;
    rec.depth = 0;
    Vector3 color;
    if (world.ManageHits(r, 0, FLT_MAX, rec, color))
    {

        if (depth < MAX_DEPTH)
        {


            if (rec.material.kr>0)
            {
                //Color sumColor = Color(0, 0, 0, 1);
                //Vector3 perfectReflection = Vector3::Reflect(r.GetDirection(), rec.normal);
                //perfectReflection.Normalize();
                //for (size_t i = 0; i < NUM_SAMPLES; i++)
                //{
                //    auto random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                //
                //    float weight = 0.f;
                //
                //    if (random < rec.material.diffuseCeoff)
                //    {
                //        //diffuse
                //        auto u1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                //        auto u2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                //
                //        Vector3 rayDir = CalculateDiffuseDirection(u1, u2, rec.normal);
                //
                //        MyRay ray = MyRay(rec.point+0.1*rec.normal, rayDir);
                //
                //        float ndotl = rec.normal.Dot(rayDir);
                //
                //        Color reflectedColor = RayColor(ray, world, depth + 1)*ndotl;
                //        
                //        sumColor += rec.material.diffuseColor*reflectedColor;
                //    }
                //
                //    else if (rec.material.diffuseCeoff < random && random < (rec.material.diffuseCeoff + rec.material.specCoeff))
                //    {
                //        auto u1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                //        auto u2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                //
                //        Vector3 rayDir = CalculateSpecularDirection(u1, u2, perfectReflection,rec.material.specularity);
                //
                //        rayDir.Normalize();
                //
                //        MyRay ray = MyRay(rec.point + 0.1 * rec.normal, rayDir);
                //
                //        float ndotl = rec.normal.Dot(rayDir);
                //
                //        weight += ndotl;
                //        Color reflectedColor = RayColor(ray, world, depth + 1) * ndotl;
                //
                //        sumColor += rec.material.specColor*reflectedColor;
                //    }
                //
                //
                //}
                //
                //sumColor = Color(sumColor.R() / NUM_SAMPLES, sumColor.G() / NUM_SAMPLES, sumColor.B() / NUM_SAMPLES, 1.0f);
                //
                //color += sumColor.ToVector3();

                Vector3 reflectedVector = Vector3::Reflect(r.GetDirection(), rec.normal);
                
                MyRay reflectedRay;
                reflectedRay = MyRay(rec.point + 0.1 * reflectedVector, reflectedVector);
                
                rec.depth++;
                
                Color reflectedColor = RayColor(reflectedRay, world, depth + 1);
                

                color += rec.material.kr * reflectedColor.ToVector3();
            }

            if (rec.material.kt > 0)
            {
                Vector3 normal = rec.normal;
                float refractionIndex = 1.0f / rec.material.ior;
                if (rec.normal.Dot(-r.GetDirection()) < 0)
                {
                    normal *= -1;
                    refractionIndex = rec.material.ior / 1.0f;

                }
                Vector3 refractedVector = Vector3::Refract(r.GetDirection(), normal, refractionIndex);

                if (refractedVector.x == 0 && refractedVector.y == 0 && refractedVector.z == 0)
                {
                    //total internal reflection
                    refractedVector = Vector3::Reflect(r.GetDirection(), normal);
                }

                MyRay refractedRay;
                refractedRay = MyRay(rec.point + 0.1 * refractedVector, refractedVector);

                Color refractedColor = RayColor(refractedRay, world, depth + 1);

                color += rec.material.kt * refractedColor.ToVector3();

            }
        }

        Color finalCol = Color(color);
        return finalCol;

    }
    Vector3 direction = r.GetDirection();
    auto t = 0.5 * (direction.y + 1.0);

    Vector3 skyboxColor = Vector3(0.5, 0.7, 1.0)*7;

    return Color(skyboxColor);
}

int main()
{
    std::vector<std::shared_ptr<Hittable>> hittables;
    char filename[64] = "output.ppm";
    std::ofstream oFile;

    // Image
    const auto aspect_ratio = float(16) / float(9);
    const int image_height = 600;
    const int image_width = image_height *aspect_ratio;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Vector3(0, 0, 0);
    auto horizontal = Vector3(viewport_width, 0, 0);
    auto vertical = Vector3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 + Vector3(0, 0, focal_length);

    oFile.open(filename, std::ios::out);

    if (!oFile.is_open())
    {
        printf("error opening file\n");
        return EXIT_FAILURE;
    }

    Material mat1 = { Color(1, 1, 1, 1) , Color(1,1,1,1), 0.1f, 0.9f, 64, 0.8, 0.2, 0, 0.80, 0.98};
    Material mat2 = { Color(1, 1, 1, 1) , Color(1,1,1,1), 0.3, 0.7f, 128, 0.2, 0.8, 1.0f,0 };
    Material mat3 = { Color(1, 1, 0, 1) , Color(1,1,1,1), 0.8, 0.2, 128, 0.5, 0.01,0,0 };

    HittableManager world;
    auto sphere1 = std::make_shared<Sphere>(Vector3(1.03, 2.2, -1.61), 0.95, mat1);
    world.AddHittable(sphere1);
    auto sphere2 = std::make_shared<Sphere>(Vector3(2.5, 1.7, 0), 0.8f, mat2);
    world.AddHittable(sphere2);
    auto rect1 = std::make_shared<HitRectangle>(Vector3(4, 0, 2.4), Vector3(0, 1, 0), Vector3(10, 10, 15.8), mat3);
    world.AddHittable(rect1);
    //auto torus = std::make_shared<Torus>(1, 0.5, Vector3(5, 1, 0.5), mat3);
    //world.AddHittable(torus);

    //loading the bunny
    //{
    //    happly::PLYData plyIn("Models/bunny.ply");
    //    std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
    //    std::vector<std::vector<size_t>> fInd = plyIn.getFaceIndices<size_t>();
    //
    //
    //}
    //
    PointLight pLight1 = {};
    pLight1.position = Vector3(0.8f, 3.28, -4.21);
    pLight1.intensity = 20;
    pLight1.color = Vector3(1, 1, 1);

    PointLight pLight2 = {};
    pLight2.position = Vector3(2, 3.28, -3.21);
    pLight2.intensity = 5;
    pLight2.color = Vector3(1, 1, 1);

    world.AddPointLight(pLight1);
    //world.AddPointLight(pLight2);

    //std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    oFile << "P3\n" + std::to_string(image_width) + " " + std::to_string(image_height) + "\n255\n";

    Camera camera(Vector3(0.88, 2, -4.13), Vector3(0.88, 2, 0), Vector3(0, 1, 0), aspect_ratio, 80);

    float* data = new float[image_height * image_width*3];

    int numberOfSamples = 24;

    std::random_device rd;

    std::mt19937 random(rd());

    srand(time(NULL));


    int count = 0;
    for (int j = image_height - 1; j >= 0; --j) 
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
    
            Color col = Color(0, 0, 0, 1);
            std::uniform_real_distribution<double> dist(0, 1.0);
    
            for (size_t s = 0; s < numberOfSamples; s++)
            {
                float u = float(i + dist(random)) / float(image_width);
                float v = float(j + dist(random)) / float(image_height);
    
                MyRay r = camera.GetRay(u, v);
                col += RayColor(r, world);
    
            }
    
            Color pixel_color = Color(col.R() / float(numberOfSamples), col.G() / float(numberOfSamples), col.B() 
                / float(numberOfSamples), 1.0f);


            data[count++] = pixel_color.R();
            data[count++] = pixel_color.G();
            data[count++] = pixel_color.B();
    
            //pixel_color = ToneReproductionReinhardt(pixel_color.ToVector3());
            //
            //oFile << std::to_string(pixel_color.R()*255.99) + " " + 
            //    std::to_string(pixel_color.G() * 255.99) + " " + std::to_string(pixel_color.B() * 255.99) + "\n";
        }
    }

    count = 0;
    float keyValue = 0;
    float logKeyValue = 0;
    //auto bytesPerLine = image_width * 3;
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            auto pixelColor = Color(data[count + 0], data[count + 1], data[count + 2], 1.0f);
            count += 3;
            logKeyValue += std::log(0.001 + CalcLuminance(pixelColor));
        }
    }

    logKeyValue /= (float(image_height) * float(image_width));
    keyValue = std::exp(logKeyValue);

    float maxLuma = FLT_MIN;


    count = 0;
    //auto bytesPerLine = image_width * 3;
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            auto pixelColor = Color(data[count + 0], data[count + 1], data[count + 2], 1.0f);

            auto curluma = CalcLuminance(pixelColor);

            if (maxLuma < curluma)
            {
                maxLuma = curluma;
            }
        }
    }

    count = 0;
    //auto bytesPerLine = image_width * 3;
    for (int j = image_height - 1; j >= 0; --j) 
    {
        for (int i = 0; i < image_width; ++i) 
        {
            auto pixelColor = Color(data[count+0], data[count+1], data[count+2], 1.0f);
            count += 3;
            //pixelColor = ToneReproductionReinhardt(pixelColor.ToVector3(), keyValue);

            //pixelColor = ToneReproductionWard(pixelColor.ToVector3(), keyValue, 400);

            pixelColor = ToneReproductionAdaptiveLogarithmic(pixelColor.ToVector3(),
                0.85,maxLuma,CalcLuminance(pixelColor), 400);

    
            oFile << std::to_string(pixelColor.R() * 255.99) + " " +
                std::to_string(pixelColor.G() * 255.99) + " " + std::to_string(pixelColor.B() * 255.99) + "\n";
        }
    }

    //stbi_write_png("output.png", image_width, image_height, 4, data, image_width*4);

    delete[] data;

	if (oFile.is_open())
	{
		oFile.close();
	}

	printf("Done");
	getchar();
}

