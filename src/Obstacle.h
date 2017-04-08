#pragma once
#include "Vec3f.h"
#include <vector>
#include "glad/glad.h"

class Obstacle
{
public:
	Vec3f position;
	GLuint vertBufferID;
	GLuint vaoID;

	virtual Vec3f nearest_point(Vec3f pt) { return Vec3f(0, 0, 0); }
    virtual void render() {}
    virtual void load() {}
    virtual void updateGPU() {}
};

class Sphere : public Obstacle
{
public:
	float radius;
	std::vector<Vec3f> verts;

	Sphere(Vec3f p, float r);
	Vec3f nearest_point(Vec3f pt) override;
	void render() override;
	void load() override;
	void updateGPU() override;
};
