#pragma once

#include "Vec3f.h"
#include <vector>
#include "glad/glad.h"
#include "Behaviour.h"
#include "Mat4f.h"

class Boid
{
public:
	Vec3f pos;
    Vec3f vel;

    Vec3f color;
	Mat4f transform;

	Boid();
	Boid(Vec3f p, Vec3f v);

	float linear_weight(Boid * b, float r_inner, float r_outer);
	Vec3f calc_heading(std::vector<Boid*> *boids, Behaviour *bhvr);
	void update(std::vector<Boid*> *boids, Behaviour *bhvr, float dt);

	Vec3f following(Boid *b, Behaviour *bhvr);
	Vec3f avoid(Boid *b, Behaviour *bhvr);
	Vec3f velocity(Boid *b, Behaviour *bhvr);
	
	void render();
    void load();
    void updateGPU();
    
	const Vec3f gravity = Vec3f(0, -9.81, 0);

	GLuint vertBufferID;
    GLuint vaoID;
};

extern void reloadColorUniform(float r, float g, float b);
