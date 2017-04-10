#pragma once

#include "Vec3f.h"
#include <vector>
#include "glad/glad.h"
#include "Behaviour.h"
#include "Mat4f.h"
#include "Obstacle.h"

class Boid
{
public:
	Vec3f pos;
    Vec3f vel;
	Vec3f heading;

    Vec3f color;
	Mat4f transform;

	Boid();
	Boid(Vec3f p, Vec3f v);

	float linear_weight(Vec3f b_pos, float r_inner, float r_outer);
	void calc_heading(std::vector<Boid*> *boids, std::vector<Obstacle*> *objs, Behaviour *bhvr);
	void update(float dt);

	Vec3f following(Boid *b, Behaviour *bhvr);
	Vec3f avoid(Vec3f b_pos, Behaviour *bhvr);
    Vec3f match_velocity(Boid *b, Behaviour *bhvr);
	
	void render();
    void load();
    void updateGPU();

    const float MIN_SPEED = 7.5f;
    const float MAX_SPEED = 10;

	GLuint vertBufferID;
    GLuint vaoID;
};

extern void reloadColorUniform(float r, float g, float b);
