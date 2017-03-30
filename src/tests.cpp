#include "glm/glm.hpp"
#include "Spring.h"
#include <iostream>

void test_create()
{
	Mass m_1 = Mass();
	m_1.mass = 1;
	m_1.pos = Vec3f(0, -1, 0);
	m_1.vel = Vec3f(0, 0, 0);
	m_1.fixed = false;

	Mass m_2 = Mass();
	m_2.mass = 1;
	m_2.pos = Vec3f(0, -2.5f, 0);
	m_2.vel = Vec3f(0, 0, 0);

	Spring s = Spring();
	s.k = 0.1f;
	s.x_rest = 1;
	s.mass_1 = &m_1;
	s.mass_2 = &m_2;

	float dt = 1.0f / 30.0f;
	while (true)
	{
		Vec3f x = s.getX(dt);
		Vec3f v = s.getVel(dt);
		Vec3f &mp = s.mass_2->pos;
		mp = x;
		s.mass_2->vel = v;

		printf("%f %f %f\n", x.x(), x.y(), x.z());
	}
	int i;
	std::cin >> i;
}

void test_create2()
{
	Mass m_1 = Mass();
	m_1.mass = 1;
	m_1.pos = Vec3f(0, 0, 0);
	m_1.vel = Vec3f(0, 0, 0);

	Mass m_2 = Mass();
	m_2.mass = 1;
	m_2.pos = Vec3f(0, -1.0f, 0);
	m_2.vel = Vec3f(0, 0, 0);

	Mass m_3 = Mass();
	m_3.mass = 1;
	m_3.pos = Vec3f(0, -2.5f, 0);
	m_3.vel = Vec3f(0, 0, 0);

	Spring s = Spring();
	s.k = 0.1f;
	s.x_rest = 1;
	s.mass_1 = &m_1;
	s.mass_2 = &m_2;

	Spring s2 = Spring();
	s2.k = 0.1f;
	s2.x_rest = 1;
	s2.mass_1 = &m_2;
	s2.mass_2 = &m_3;

	float dt = 1.0f / 30.0f;
	while (true)
	{
		s.update(dt);
		s2.update(dt);
		Vec3f x = s2.mass_2->vel;;

		printf("%f %f %f\n", x.x(), x.y(), x.z());
	}
	int i;
	std::cin >> i;
}

int main(int argc, char** argv)
{
	//test_create2();
	test_create();
	return 0;
}
