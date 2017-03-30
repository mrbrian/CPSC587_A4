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

	std::cin >> w_avoid;
	std::cin >> w_follow;
	std::cin >> w_match;

	file.close();
}