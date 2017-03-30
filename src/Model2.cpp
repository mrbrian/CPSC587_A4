#include "Model.h"

void Model2::init()
{
	int num_chains = 10;
	Vec3f start = Vec3f(0, 2, 0);
	float scale = 0.3f;
	Vec3f offset = Vec3f(1, -1, 0).normalized() * scale;
	Mass *prev_mass = 0;

	for (int i = 0; i < num_chains + 1; i++)
	{
		Mass *m = new Mass();
		m->mass = 0.125f;
		m->pos = start + offset * i;
		m->vel = Vec3f(0, 0, 0);
		
		if (i == 0)
			m->fixed = true;

		if (m && prev_mass)
		{
			Spring *s = new Spring();
			s->k = 100;
			s->damp = 0.5f;
			s->x_rest = scale;
			s->mass_1 = prev_mass;
			s->mass_2 = m;
			s->color = Vec3f(1, 0, 0);
			springs.push_back(s);
		}

		masses.push_back(m);
		prev_mass = m;
	}

	for (int i = 0; i < springs.size(); i++)
		springs[i]->load();
}
