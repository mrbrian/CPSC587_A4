#include <iostream>
#include <fstream>
#include "Model.h"


void Model1::init()
{
	Model();

	bhvr = read_input();
	for (int i = 0; i < bhvr.num_boids; i++)
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

	for (int i = 0; i < 2; i++)
	{
		Obstacle *a = new Sphere(
			//Vec3f(RAND_1(), RAND_1(), RAND_1()),
			Vec3f(0,0,0),
			0.5f);
		objects.push_back(a);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		Obstacle &b = *objects[i];
		b.load();
	}

}

void Model2::init()
{
	Model();

	Sphere *obj = new Sphere(Vec3f(0, 0, 0), 0.25f);
	objects.push_back(obj);

	bhvr = read_input();
	Boid *a = new Boid(
		Vec3f(0, 0.00001f, 0),
		Vec3f(-5, 0, 0)
	);
	boids.push_back(a);

	Boid *b = new Boid(
		Vec3f(1, 0.0002f, 0),
		Vec3f(5, 0, 0)
	);
	boids.push_back(b);

	obj->load();

	for (int i = 0; i < boids.size(); i++)
	{
		Boid &b = *boids[i];
		b.load();
	}
}

Model::Model()
{
	init();
}

void Model::render()
{
	for (int i = 0; i < boids.size(); i++)
	{
		Boid &b = *boids[i];

		b.render();
	}
	for (int i = 0; i < objects.size(); i++)
	{
		Obstacle &o = *objects[i];

		o.render();
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

        b.update(&boids, &objects, &bhvr, dt);
    }
}

Behaviour Model::read_input()
{	
    std::fstream file;
    int num_boids;
    int num_objs;

    file.open("input.txt");
    if (!file.is_open())
        return Behaviour();
    file >> num_boids;
    file >> num_objs;

	float w_avoid;
	float w_follow;
	float w_match;

    float r_avoid;
    float r_follow;
	float r_match;
	float fov;

    file >> w_avoid;
    file >> w_follow;
    file >> w_match;

    file >> r_avoid;
    file >> r_follow;
    file >> r_match;

    file >> fov;

    file.close();

    return Behaviour(
        num_boids,
        num_objs,
		r_avoid, 
		r_follow, 
		r_match,

		w_avoid,
		w_follow,
		w_match,

		fov
    );
}
