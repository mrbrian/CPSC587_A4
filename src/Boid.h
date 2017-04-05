#pragma once

#include "Vec3f.h"
#include <vector>
#include "glad/glad.h"
#include "Behaviour.h"

class Boid
{
public:
	Boid();

	Vec3f pos;
    Vec3f vel;

    Vec3f color;
    float linear_weight(Boid *b, Behaviour *bhvr);
    void update(std::vector<Boid*> *boids, Behaviour *bhvr);

    /*
    void following();
    void avoid();
    void velocity();
    void render();
    void load();
    void updateGPU();
    */
	const Vec3f gravity = Vec3f(0, -9.81, 0);

	GLuint vertBufferID;
    GLuint vaoID;
};

