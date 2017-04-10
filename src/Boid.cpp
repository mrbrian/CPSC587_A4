#include "Boid.h"
#include <algorithm>
#include "Obstacle.h"

Boid::Boid()
{
	transform = Mat4f({
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	});
	pos = Vec3f(0, 0, 0);
	vel = Vec3f(0, 0, 0);
}

Boid::Boid(Vec3f p, Vec3f v)
{
    Boid();
	transform = Mat4f({
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	});
	pos = p;
	vel = v;	
}

float Boid::linear_weight(Vec3f b_pos, float r_inner, float r_outer)
{
    float r = (b_pos - pos).length();

    float result = (r - r_inner) / (r_outer - r_inner);
    return result;
}

Vec3f Boid::following(Boid *b, Behaviour *bhvr)
{
	Vec3f dir = b->pos - pos;
	float w = 1 - linear_weight(b->pos, bhvr->rad_v, bhvr->rad_f);
    color[1] = w;

    return dir * w;
}

Vec3f Boid::avoid(Vec3f b_pos, Behaviour *bhvr)
{
	Vec3f result = pos - b_pos;
	float lin_w = linear_weight(b_pos, 0, bhvr->rad_a);
	float w = 1 / pow(lin_w, 2) - 1;
	result.normalize();

    color[0] = w;
	return result * w;
}

Vec3f Boid::match_velocity(Boid *b, Behaviour *bhvr)
{
    float w = 1 - linear_weight(b->pos, bhvr->rad_a, bhvr->rad_v);

    color[2] = w;
    return b->vel * w;
}

bool within_radius(Boid *a, Vec3f b_p, float r)
{
	float r_sq = (b_p - a->pos).lengthSquared();
	return (r_sq <= r * r);
}

bool visible_range(Boid *a, Boid *b, float rad)
{
	Vec3f dir = (b->pos - a->pos).normalized();
    Vec3f unit_v = a->vel.normalized();
	double phi = acos(unit_v.dotProduct(dir));
	return (phi < rad);
}

void Boid::calc_heading(std::vector<Boid*> *boids, std::vector<Obstacle*> *objs, Behaviour *bhvr)
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
		if (!visible_range(this, &b, bhvr->fov))
			continue;

		if (within_radius(this, b.pos, bhvr->rad_a))
		{
			h_a += avoid(b.pos, bhvr);
			continue;
		}
		if (within_radius(this, b.pos, bhvr->rad_f))
		{
			h_f += following(&b, bhvr);
			num_follow++;
		}
		if (within_radius(this, b.pos, bhvr->rad_v))
            h_v += match_velocity(&b, bhvr);
	}

	for (int j = 0; j < objs->size(); j++)
	{
		Obstacle &b = *(*objs)[j];
		Vec3f pt = b.nearest_point(pos);
		if (within_radius(this, pt, bhvr->rad_a))
		{
			h_a += avoid(pt, bhvr);
			continue;
		}
	}
	if (num_follow > 0)
		h_f *= 1.0f / num_follow;
	if (num_match > 0)
		h_v *= 1.0f / num_match;

	h_f = h_f - pos;

    Vec3f h = a_a * h_a + a_f * h_f + a_v * (h_v - v);
	heading = h;    
}

void Boid::update(float dt)
{
	Vec3f unit_vel = vel.normalized();
	Vec3f perp_accel = heading - unit_vel.dotProduct(heading) * unit_vel;
	Vec3f par_accel = unit_vel.dotProduct(heading) * unit_vel;
	
	Vec3f t = vel.normalized();
	Vec3f n = perp_accel.normalized();
	Vec3f b = t.crossProduct(n).normalized();

    vel += (perp_accel + par_accel) * dt;
    if (vel.lengthSquared() < MIN_SPEED * MIN_SPEED)
        vel = vel.normalized() * MIN_SPEED;
    if (vel.lengthSquared() > MAX_SPEED * MAX_SPEED)
		vel = vel.normalized() * MAX_SPEED;

	pos += vel * dt;
	Vec3f p = pos;

	transform = Mat4f({        // create the frenet frame matrix
		b.x(), n.x(), t.x(), p.x(),
		b.y(), n.y(), t.y(), p.y(),
		b.z(), n.z(), t.z(), p.z(),
		0, 0, 0, 1
	});
}

void Boid::render()
{
	// Use VAO that holds buffer bindings
	// and attribute config of buffers
	glBindVertexArray(vaoID);
	updateGPU();

	// Draw Quads, start at vertex 0, draw 4 of them (for a quad)
   // color.normalize();
    reloadColorUniform(color.x(), color.y(), color.z());
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
    float scale = 0.25f;
	Vec3f verts[3] = {
        Vec3f(-1 / sqrt(2),0,-1 / sqrt(2)) * scale,
        Vec3f(0, 0, 1) * scale ,
        Vec3f(1 / sqrt(2),0,-1 / sqrt(2)) * scale
	};

	for (int i = 0; i < 3; i++)
	{
        verts[i] = transform * verts[i];
	}
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
