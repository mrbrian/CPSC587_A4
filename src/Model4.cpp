#include "Model.h"

void Model4::init()
{
    int w = 10;
    int h = 10;
    float scale = 0.3f;
	float max_dist_squared = 2 * scale * scale;
    Vec3f start = Vec3f(0, 2, 0);
	Mass *mass_heap = new Mass[w * h];
	Mass *prev_mass = 0;

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
		{
			Mass *m = &mass_heap[y * w + x];
            m->mass = 0.05f;
            m->pos = start + Vec3f(x, -y, RAND_1() / 100) * scale;
			m->vel = Vec3f(0, 0, 0);

			masses.push_back(m);
		}

    masses[0]->fixed = true;

	for (int i = 0; i < masses.size(); i++)
	{
		Mass *a = masses[i];

		for (int j = 0; j < masses.size(); j++)
		{
			if (i == j)
				continue;

			Mass *b = masses[j];
			float dist_sq = (b->pos - a->pos).lengthSquared();

			if (dist_sq > max_dist_squared + scale / 100)
				continue;

			Spring *s = new Spring();
            s->k = 80;
            s->damp = 0.05f;
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
