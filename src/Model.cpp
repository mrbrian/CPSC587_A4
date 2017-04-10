#include <iostream>
#include <fstream>
#include "Model.h"
#include <string>
#include <sstream>

void Model1::init()
{
	Model();

	bhvr = read_input();
	for (int i = 0; i < bhvr.num_boids; i++)
	{
		Boid *a = new Boid(
			Vec3f(RAND_1(), RAND_1(), RAND_1()).normalized() * bhvr.distr,
			Vec3f(RAND_1(), RAND_1(), RAND_1())
		);
		boids.push_back(a);
	}

	for (int i = 0; i < boids.size(); i++)
	{
		Boid &b = *boids[i];
		b.load();
	}

	// boundary sphere
	objects.push_back(new Sphere(
		Vec3f(0, 0, 0),
		Vec3f(0.3f, 0.3f, 0.3f),
		bhvr.world));

	for (int i = 0; i < bhvr.num_objs; i++)
	{
		float dist = (float)i / bhvr.num_objs * bhvr.world;
		Obstacle *a = new Sphere(
			Vec3f(RAND_1(), RAND_1(), RAND_1()).normalized() * dist,
			Vec3f(1, 0, 0),
			2);
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

	Sphere *obj = new Sphere(Vec3f(0, 0, 0), Vec3f(0.3f, 0.3f, 0.3f), 0.25f);
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

		b.calc_heading(&boids, &objects, &bhvr);
	}

	for (int i = 0; i < boids.size(); i++)
    {
        Boid &b = *boids[i];

        b.update(dt);
    }
}

float file_read_value(std::fstream *file)
{
	float result;
	std::string str;

	std::getline(*file, str);

	std::stringstream ss(str);
	
	ss >> result;
	return result;
}

Behaviour Model::read_input()
{	
    std::fstream file;
    int num_boids;
    int num_objs;

    file.open("input.txt");
    if (!file.is_open())
        return Behaviour();
	num_boids = file_read_value(&file);
    num_objs = file_read_value(&file);

	float w_avoid;
	float w_follow;
	float w_match;

    float r_avoid;
    float r_follow;
	float r_match;
	float fov;

    w_avoid = file_read_value(&file);
    w_follow = file_read_value(&file);
    w_match = file_read_value(&file);

    r_avoid = file_read_value(&file);
    r_follow = file_read_value(&file);
    r_match = file_read_value(&file);

	fov = file_read_value(&file);
	float world = file_read_value(&file);
	float distr = file_read_value(&file);

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

		fov,
		world,
		distr
    );
}
