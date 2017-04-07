#include "Obstacle.h"

Vec3f Sphere::nearest_point(Vec3f pt) 
{
	Vec3f p = pt - position;
	return p * radius;
}