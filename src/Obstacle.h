#pragma once
#include "Vec3f.h"

class Obstacle
{
public:
	Vec3f position;

	virtual Vec3f nearest_point(Vec3f pt);
	virtual void render();
	virtual void load();
	virtual void updateGPU();
};

class Sphere : public Obstacle
{
public:
	float radius;

	Vec3f nearest_point(Vec3f pt) override;
	void render() override;
	void load() override;
	void updateGPU() override;
};

