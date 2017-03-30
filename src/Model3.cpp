#include "Model.h"

void Model3::init()
{
    int w = 5;
    int h = 5;
    int depth = 5;

	float scale = 0.3f;
	float max_dist_squared = 3 * scale * scale;
    Vec3f start = Vec3f(-w, -h, -depth) * 0.5f * scale;
	Mass *prev_mass = 0;

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			for (int z = 0; z < depth; z++)
			{
				Mass *m = new Mass();
				m->mass = 0.25f;
				m->pos = start + Vec3f(x, y, z) * scale;
				m->vel = Vec3f(0, 0, 0);

				masses.push_back(m);
			}

	float rad = 3.1415926535897f / 3;
	Mat4f rot = Mat4f(
	{
		cos(rad),		-sin(rad),	0,		0,
		sin(rad),		cos(rad),	0,		1,
		0,				0,			1,		0,
		0,				0,			0,		1,
	}
	);

	for (int i = 0; i < masses.size(); i++)
	{
		Mass *m = masses[i];
		m->pos = rot * m->pos;
	}

    for (int i = 0; i < masses.size(); i++)
	{
		Mass *a = masses[i];

        for (int j = 0; j < masses.size(); j++)
		{
			if (i == j)
				continue; 

			Mass *b = masses[j];			
			float dist_sq = (b->pos - a->pos).lengthSquared();

			if (dist_sq > max_dist_squared)
				continue;

			Spring *s = new Spring();
            s->k = 80;
            s->damp = 0.125f;
			s->x_rest = sqrt(dist_sq);
			s->mass_1 = a;
			s->mass_2 = b;
			s->color = Vec3f(1, 0, 0);
			springs.push_back(s);
		}
	}

    for (int i = 0; i < springs.size(); i++)
        springs[i]->load();
}

void Model3::update(float dt)
{
    float floor_y = -1.0f;

	for (int i = 0; i < springs.size(); i++)
		springs[i]->applyForce(dt);

	for (int i = 0; i < masses.size(); i++)
	{
		masses[i]->addGravity(dt);
		masses[i]->resolveForce(dt);
		Vec3f pos = masses[i]->pos;

		// collide with floor plane
		if (pos.y() < floor_y) 
		{
			pos.set(pos.x(), floor_y, pos.z());
			masses[i]->pos = pos;

			// damp horizontal velocity along ground
			masses[i]->vel = Vec3f(
				masses[i]->vel[0] / 1.02f,
				masses[i]->vel[1],
				masses[i]->vel[2] / 1.02f
			);
		}
	}
}
