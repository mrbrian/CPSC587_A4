#include "Obstacle.h"
#include <vector>

Vec3f Sphere::nearest_point(Vec3f pt) 
{
	Vec3f p = pt - position;
	return p * radius;
}

void Sphere::render()
{
}

void Sphere::load()
{
    int num_phi = 8;
    int num_theta = 8;
    int radius = 1;

    std::vector<Vec3f> verts;

    for (int i = 0; i < num_phi; i++)
    {
        for (int j = 0; j < num_theta; j++)
        {
            //verts.push_back();
        }
    }
}

void Sphere::updateGPU()
{
}
