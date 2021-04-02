#include "Torus.h"

Torus::Torus(float sweepRadius, float tubeRadius, Vector3 pos, Material mat) :Hittable(mat)
{
	world = Matrix::CreateTranslation(pos);
    sweepRad = sweepRadius;
    this->tubeRad = tubeRadius;
    this->pos = pos;
	LoadTorusMesh();

	boundingSphere.Center = Vector3(0,0,0);
}


Torus::~Torus()
{
}

bool Torus::Hit(const MyRay& ray, float tMin, float tMax, HitRecord& record)
{
    float t = ComputeIntersection(ray, record.t);

    if (t > 0)
    {
        record.t = t;
        record.point = ray.GetPointAtParameter(record.t);
        record.material = mat;
        return true;
    }

    return false;
}

void Torus::LoadTorusMesh()
{

	std::ifstream ifile("Torus.obj");

	std::string line; //line that stores file data

	float maxDist = FLT_MIN;
	Vector3 origin = Vector3(0, 0, 0);
	//check if the file exists
	if (ifile.is_open())
	{
		//making a list of the position, normals and uvs

		//num of verts and indices
		UINT vertCount = 0;
		UINT indexCount = 0;

		while (std::getline(ifile, line))
		{
			std::vector<std::string> words; //this holds all the individual characters of the line

			size_t pos = 0;
			size_t curPos = 0;

			//splitting the string with the spacebar
			while (pos <= line.length())
			{
				//finding the space string
				//taking a substring from that point
				//storing that substring in the list
				pos = line.find(" ", curPos);
				std::string word = line.substr(curPos, (size_t)(pos - curPos));
				curPos = pos + 1;
				words.emplace_back(word);
			}

			if (line.find("v ") == 0)
			{
				//storing the position if the line starts with "v"
				positions.emplace_back(XMFLOAT3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
			}

			if (line.find("vn") == 0)
			{
				//storing the normals if the line starts with "vn"
				normals.emplace_back(XMFLOAT3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
			}

			if (line.find("vt") == 0)
			{
				//storing the textures if the line starts with "vt"
				uvs.emplace_back(XMFLOAT2(std::stof(words[1]), std::stof(words[2])));
			}

			if (line.find("f") == 0)
			{
				//storing the faces if the line starts with "f"

				//this the the list of the faces on this line
				std::vector<std::vector<std::string>> listOfFaces;

				listOfFaces.reserve(10);

				//looping through all the faces
				for (int i = 0; i < words.size() - 1; i++)
				{
					std::vector<std::string> face; //holds the individial data of each face
					face.reserve(10);
					pos = 0;
					curPos = 0;

					//splitting each vertex further to seperate it based on a '/' character
					while (pos <= words[(size_t)i + 1].length())
					{
						(pos = words[(size_t)i + 1].find("/", curPos));
						std::string word = words[(size_t)i + 1].substr(curPos, pos - curPos);
						curPos = pos + 1;
						face.emplace_back(word);
					}

					//adding this face to the list of faces
					listOfFaces.emplace_back(face);
				}

				//first vertex
				Vertex v1;
				v1.Position = positions[(size_t)std::stoi(listOfFaces[0][0]) - 1];
				v1.normal = normals[(size_t)std::stoi(listOfFaces[0][2]) - 1];
				v1.uv = uvs[(size_t)std::stoi(listOfFaces[0][1]) - 1];

				//second vertex
				Vertex v2;
				v2.Position = positions[(size_t)std::stoi(listOfFaces[1][0]) - 1];
				v2.normal = normals[(size_t)std::stoi(listOfFaces[1][2]) - 1];
				v2.uv = uvs[(size_t)std::stoi(listOfFaces[1][1]) - 1];

				//third vertex
				Vertex v3;
				v3.Position = positions[(size_t)std::stoi(listOfFaces[2][0]) - 1];
				v3.normal = normals[(size_t)std::stoi(listOfFaces[2][2]) - 1];
				v3.uv = uvs[(size_t)std::stoi(listOfFaces[2][1]) - 1];

				//since the texture space starts at top left, it its probably upside down
				v1.uv.y = 1 - v1.uv.y;
				v2.uv.y = 1 - v2.uv.y;
				v3.uv.y = 1 - v3.uv.y;

				//since the coordinates system are inverted we have to flip the normals and 
				//negate the z axis of position
				v1.normal.z *= -1;
				v2.normal.z *= -1;
				v3.normal.z *= -1;

				v1.Position.z *= -1;
				v2.Position.z *= -1;
				v3.Position.z *= -1;

				//we have to flip the winding order
				vertices.emplace_back(v1);
				vertices.emplace_back(v3);
				vertices.emplace_back(v2);

				if (Vector3::Distance(origin, v1.Position) > maxDist)
				{
					maxDist = Vector3::Distance(origin, v1.Position);
				}

				if (Vector3::Distance(origin, v2.Position) > maxDist)
				{
					maxDist = Vector3::Distance(origin, v2.Position);
				}

				if (Vector3::Distance(origin, v3.Position) > maxDist)
				{
					maxDist = Vector3::Distance(origin, v3.Position);
				}

				//adding indices
				indices.emplace_back(vertCount); vertCount++;
				indices.emplace_back(vertCount); vertCount++;
				indices.emplace_back(vertCount); vertCount++;


				//if it has a potential 4th vertex, add it, but ignore n-gons
				if (listOfFaces.size() > 3)
				{
					//fourth vertex
					Vertex v4;
					v4.Position = positions[(size_t)std::stoi(listOfFaces[3][0]) - 1];
					v4.normal = normals[(size_t)std::stoi(listOfFaces[3][2]) - 1];
					v4.uv = uvs[(size_t)std::stoi(listOfFaces[3][1]) - 1];

					//do the same handedness conversion
					v4.Position.z *= -1;
					v4.normal.z *= -1;
					v4.uv.y = 1 - v4.uv.y;

					//adding the triangle
					vertices.emplace_back(v1);
					vertices.emplace_back(v4);
					vertices.emplace_back(v3);
					if (Vector3::Distance(origin, v4.Position) > maxDist)
					{
						maxDist = Vector3::Distance(origin, v4.Position);
					}

					//adding indices
					indices.emplace_back(vertCount); vertCount++;
					indices.emplace_back(vertCount); vertCount++;
					indices.emplace_back(vertCount); vertCount++;

				}
			}


		}
		ifile.close();
	}
	boundingSphere.Radius = maxDist;
}

bool Torus::ComputeNormal(const MyRay& ray, float& t)
{
	return false;
}

float Sign(float num)
{
    if (num > 0) return 1;
    if (num < 0) return -1;
    return 0;
}

float Torus::ComputeIntersection(const MyRay& ray, float& t)
{
	auto tempRay = ray;
	tempRay.TransformRayToLocalSpace(world);
	Ray newRay;
	newRay.direction = tempRay.GetDirection();
	newRay.position = tempRay.GetOrigin();
	float dist = 0;
	if (!newRay.Intersects(boundingSphere,dist))
	{
		return -1;
	}

	bool intersects = false;
	auto triCount = indices.size()/ 3.0f;
	static float newDist = t;
	for (size_t i = 0; i < triCount; i++)
	{
		UINT i0 = indices[i * 3 + 0];
		UINT i1 = indices[i * 3 + 1];
		UINT i2 = indices[i * 3 + 2];

		auto v0 = XMLoadFloat3(&vertices[i0].Position);
		auto v1 = XMLoadFloat3(&vertices[i1].Position);
		auto v2 = XMLoadFloat3(&vertices[i2].Position);

		if (newRay.Intersects(v0, v1, v2, newDist))
		{
			if (newDist < t)
			{
				t = newDist;
			}
		}
	}

	return t;
}
