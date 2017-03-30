#pragma once

#include "Vec3f.h"
#include <vector>
#include "glad/glad.h"

class Mass
{
public:
	Mass();

	Vec3f force;
	Vec3f pos;
	Vec3f vel;
	float mass;
	bool fixed;

	Vec3f color;
	virtual void render();
	virtual void load();
	virtual void updateGPU();
	virtual void resolveForce(float dt);
	void addGravity(float dt);
	const Vec3f gravity = Vec3f(0, -9.81, 0);

	GLuint vertBufferID;
	GLuint vaoID;;
};

