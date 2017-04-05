#include <iostream>
#include <fstream>
#include "Model.h"

void Model::render()
{
}

void Model::init()
{
}

void Model::update(float dt)
{
    for (int i = 0; i < boids.size(); i++)
    {
        Boid b = boids[i];

        //b.update();
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

    std::cin >> w_avoid;
    std::cin >> w_follow;
    std::cin >> w_match;

    std::cin >> r_avoid;
    std::cin >> r_follow;
    std::cin >> r_match;

	file.close();
}
