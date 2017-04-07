#include <iostream>
#include <fstream>
#include "Model.h"


void Model1::init()
{
	Model::Model();

	for (int i = 0; i < 3; i++)
	{
		Boid *a = new Boid(
			Vec3f(RAND_1(), RAND_1(), RAND_1()),
			Vec3f(RAND_1(), RAND_1(), RAND_1())
		);
		boids.push_back(a);
	}

	for (int i = 0; i < boids.size(); i++)
	{
		Boid &b = *boids[i];
		b.load();
	}
}

Model::Model()
{
	bhvr = Behaviour(
		1, 2, 3,
		3, 2, 1, 
		160);
	init();
}

void Model::render()
{
	for (int i = 0; i < boids.size(); i++)
	{
		Boid &b = *boids[i];

		b.render();
	}
}

void Model::init()
{
}

void Model::update(float dt)
{
    for (int i = 0; i < boids.size(); i++)
    {
        Boid &b = *boids[i];

        b.update(&boids, &bhvr, dt);
    }
}

void Model::read_input()
{	
	std::fstream file;
	int num_boids;

	file.open("example.txt");

	std::cin >> num_boids;

	float w_avoid;
	float w_follow;
	float w_match;

    float r_avoid;
    float r_follow;
	float r_match;
	float fov;

    std::cin >> w_avoid;
    std::cin >> w_follow;
    std::cin >> w_match;

    std::cin >> r_avoid;
    std::cin >> r_follow;
	std::cin >> r_match;

	std::cin >> fov;

	Behaviour b = Behaviour(
		r_avoid, 
		r_follow, 
		r_match,

		w_avoid,
		w_follow,
		w_match,

		fov
		);

	file.close();
}
