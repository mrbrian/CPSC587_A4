#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "Boid.h"

#define M_PI 3.14159265358979323846

void test_create()
{
	Behaviour bhvr = Behaviour(1, 2, 3, 3, 2, 1, 160.0f * M_PI / 180);
	std::vector<Boid*> boids;

	Boid a = Boid(
		Vec3f(0,0,0), 
		Vec3f(0,0,1));
	Boid b = Boid(
		Vec3f(1, 0, -1),
		Vec3f(0, 0, 1));
	boids.push_back(&a);
	boids.push_back(&b);

	Vec3f h = a.calc_heading(&boids, &bhvr);
	printf("%f %f %f", h.x(), h.y(), h.z());
	
}

int main(int argc, char** argv)
{
	test_create();
	return 0;
}
