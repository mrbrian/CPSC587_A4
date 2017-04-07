#include "Obstacle.h"

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
    int max_phi = 0;
    int max_theta = 0;
    int radius = 1;
/*
    for (int i = 0; i < max_phi; i++)
    {
        for (int  = 0; i < max_phi; i++)
        {
            for (int i = 0; i < max_phi; i++)
            {

            }
        }
    }*/
}

void Sphere::updateGPU()
{
}
