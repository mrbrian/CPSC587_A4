#include "Boid.h"
#include <algorithm>

Boid::Boid()
{
}

Boid::Boid(Vec3f p, Vec3f v)
{
	pos = p;
	vel = v;
}

float Boid::linear_weight(Boid *b, float r_inner, float r_outer)
{
    float r = (b->pos - pos).length();

    float result = (r - r_inner) / (r_outer - r_inner);
    return result;
}

Vec3f Boid::following(Boid *b)
{
	Vec3f dir = b->pos - pos;
	return dir;
}

Vec3f Boid::avoid(Boid *b)
{
	Vec3f result;

	result.normalize();
	return result;
}

Vec3f Boid::velocity(Boid *b)
{
	return b->pos;
}

bool within_radius(Boid *a, Boid *b, float r)
{
	float r_sq = (b->pos - a->pos).lengthSquared();
	return (r_sq <= r * r);
}

bool visible_range(Boid *a, Boid *b, float rad)
{
	Vec3f dir = (b->pos - a->pos).normalized();
	Vec3f unit_v = a->vel.normalized();
	double phi = acos(unit_v.dotProduct(dir));
	return (phi < rad);
}

Vec3f Boid::calc_heading(std::vector<Boid*> *boids, Behaviour *bhvr)
{
	float a_a = bhvr->weight_a;
	float a_f = bhvr->weight_f;
	float a_v = bhvr->weight_v;
	int num_follow = 0;
	int num_match = 0;

	Vec3f h_a = Vec3f(0, 0, 0);

	Vec3f h_f = Vec3f(0, 0, 0);
	Vec3f h_v = Vec3f(0, 0, 0);
	Vec3f v = vel;

	for (int j = 0; j < boids->size(); j++)
	{
		Boid &b = *(*boids)[j];

		if (&b == this)
			continue;

		if (within_radius(this, &b, bhvr->rad_f)
			&& visible_range(this, &b, bhvr->fov))
		{
			h_f += following(&b);
			num_follow++;
		}
		if (within_radius(this, &b, bhvr->rad_v))
			h_v += velocity(&b); 
					
		if (within_radius(this, &b, bhvr->rad_a))
			h_a += avoid(&b);
	}
	if (num_follow > 0)
		h_f *= 1.0f / num_follow;
	if (num_match > 0)
		h_v *= 1.0f / num_match;

	h_f = h_f - pos;

	Vec3f h = a_a * h_a + a_f * h_f + a_v * (h_v - v);
	return h;
}

void Boid::update(std::vector<Boid*> *boids, Behaviour *bhvr)
{
	Vec3f heading = calc_heading(boids, bhvr);
	Vec3f accel = heading - vel;
}
