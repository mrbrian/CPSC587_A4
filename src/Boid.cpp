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

Vec3f Boid::following(Boid *b, Behaviour *bhvr)
{
	Vec3f dir = b->pos - pos;
	float w = linear_weight(b, bhvr->rad_a, bhvr->rad_f);
	return dir * w;
}

Vec3f Boid::avoid(Boid *b, Behaviour *bhvr)
{
	Vec3f result = pos - b->pos;
	float w = pow(1 - linear_weight(b, 0, bhvr->rad_a), 2);
	result.normalize();
	return result * w;
}

Vec3f Boid::velocity(Boid *b, Behaviour *bhvr)
{
	Vec3f dir = b->vel - pos;
	float w = linear_weight(b, bhvr->rad_a, bhvr->rad_v);

	return b->vel * w;
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

		if (within_radius(this, &b, bhvr->rad_a))
		{
			h_a += avoid(&b, bhvr);
			continue;	
		}
		if (within_radius(this, &b, bhvr->rad_f)
			&& visible_range(this, &b, bhvr->fov))
		{
			h_f += following(&b, bhvr);
			num_follow++;
		}
		if (within_radius(this, &b, bhvr->rad_v))
			h_v += velocity(&b, bhvr);
					
	}
	if (num_follow > 0)
		h_f *= 1.0f / num_follow;
	if (num_match > 0)
		h_v *= 1.0f / num_match;

	h_f = h_f - pos;

	Vec3f h = a_a * h_a + a_f * h_f + a_v * (h_v - v);
	return h;
}

void Boid::update(std::vector<Boid*> *boids, Behaviour *bhvr, float dt)
{
	Vec3f heading = calc_heading(boids, bhvr);
	Vec3f accel = heading - vel;

	Vec3f f = vel;
	Vec3f up = f.crossProduct(accel);
	Vec3f norm = accel;

	vel += accel * dt;
	vel += gravity * dt;
	pos += vel * dt;
}

void Boid::render()
{
	// Use VAO that holds buffer bindings
	// and attribute config of buffers
	glBindVertexArray(vaoID);
	updateGPU();

	// Draw Quads, start at vertex 0, draw 4 of them (for a quad)
	//reloadColorUniform(1, 1, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Boid::load()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertBufferID);
	updateGPU();
}

void Boid::updateGPU()
{
	Vec3f verts[3] = {
		Vec3f(-1 / sqrt(2),0,-1 / sqrt(2)),
		Vec3f(0, 1, 0),
		Vec3f(1 / sqrt(2),0,-1 / sqrt(2))
	};

	//upload to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vec3f) * 3, // byte size of Vec3f, 2 of them
		&verts,      // pointer (Vec3f*) to contents of verts
		GL_STREAM_DRAW);   // Usage pattern of GPU buffer

	glVertexAttribPointer(0,        // attribute layout # above
		3,        // # of components (ie XYZ )
		GL_FLOAT, // type of components
		GL_FALSE, // need to be normalized?
		0,        // stride
		(void *)0 // array buffer offset
	);

	glEnableVertexAttribArray(0);

}
